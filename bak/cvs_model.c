#include "cvs_model.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define CVS_PI 3.14159265358979323846f

int main(void)
{
    CVS_Simulation cvs_simulation = {
        .t                = 0.0,
        .dt               = 1e-3,
        .duration         = 40.0,

        .output_file_path = "output.csv",
        .output_file      = NULL,
    };
    
    CVS_Parameters cvs_parameters = {
        .heart_rate = 60.0f,
    };
    
    CVS_State cvs_state           = {0};

    cvs_simulation_init(&cvs_simulation, &cvs_parameters, &cvs_state);
    cvs_simulation_run(&cvs_simulation, &cvs_parameters, &cvs_state);
        
    return 0;
}

void cvs_simulation_init(CVS_Simulation *sim, CVS_Parameters *params, CVS_State *state)
{
    sim->output_file = fopen(sim->output_file_path, "w");
    if (sim->output_file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", sim->output_file_path);
        exit(1);
    }

    cvs_write_output_header(sim);

    params->e_LV_max = 2.6574f;
    params->e_RV_max = 0.4333f;
    
    params->V0[CHAMBER_LV] = 10.0f;
    params->V0[CHAMBER_RV] = 10.0f;

    // Admittances
    params->g[CHAMBER_RA] = 891.5756f; // RA -> RV
    params->g[CHAMBER_RV] = 211.4583f; // RV -> P
    params->g[CHAMBER_P]  = 6.5029f;   // P  -> LA
    params->g[CHAMBER_LA] = 947.8676f; // LA -> LV
    params->g[CHAMBER_LV] = 235.9235f; // LV -> AO
    params->g[CHAMBER_AO] = 1.0095f;   // AO -> V
    params->g[CHAMBER_V]  = 86.0398f;  // V  -> RA

    // Elastances
    params->e[CHAMBER_RA] = 0.0462f;
    params->e[CHAMBER_RV] = 0.4333f;
    params->e[CHAMBER_P]  = 0.1077f;
    params->e[CHAMBER_LA] = 0.0938f;
    params->e[CHAMBER_LV] = 2.6574f;
    params->e[CHAMBER_AO] = 0.7215f;
    params->e[CHAMBER_V]  = 0.0026f;

    // Left ventricle elastance model parameters
    params->LV_a     = 0.4641f;
    params->LV_b     = 0.018f;
    params->LV_c     = 145.0f;
    params->LV_d     = 0.1235f;
    params->LV_c1    = 461.4567f;
    params->LV_c2    = 126.2886f;
    params->LV_c3    = 333.3319f;
    params->LV_c4    = 0.3789f;
    params->LV_beta  = 1.9f;
    params->LV_gamma = 37.0f;
    params->LV_delta = 38.9f;
    params->LV_alpha = 1.5f;

    // Right ventricle elastance model parameters
    const float HR = params->heart_rate;
    
    params->RV_k0 = (-0.0042f * HR * HR - 0.0417f * HR + 257.5f) / 1000.0f;
    params->RV_k1 = (0.0158f  * HR * HR - 5.0417f * HR + 445.5f) / 1000.0f;

    params->RV_c     = 150.1f;
    params->RV_a3    = 0.03752f;
    params->RV_b3    = 0.03776f;
    params->RV_a_phi = 0.9f;
    params->RV_b_phi = 0.25f;

    state->V[CHAMBER_V]  = 5000.0f;    
}

void cvs_simulation_close(CVS_Simulation *sim)
{
    if (sim->output_file != NULL) {
        fclose(sim->output_file);
        sim->output_file = NULL;
    }
}

void cvs_simulation_run(CVS_Simulation *sim, CVS_Parameters *params, CVS_State *state)
{
    while (sim->t < sim->duration) {
        cvs_simulation_step(sim, params, state);
    }

    cvs_simulation_close(sim);
}

void cvs_simulation_step(CVS_Simulation *sim, CVS_Parameters *params, CVS_State *state)
{
    sim->t += sim->dt;

    // Update time dependent elastances values
    cvs_calculate_elastances(sim, params, state);

    // Calculate Pressures
    cvs_calculate_pressures(params, state);

    // Calculate Flows
    cvs_calculate_flows(params, state);

    // Calculate Volumes
    cvs_calculate_volumes(sim, state);

    // Write states to output file
    cvs_write_output(sim, state);
}

void cvs_calculate_elastances(CVS_Simulation *sim, CVS_Parameters *params, CVS_State *state)
{
    const float T     = 60.0f / params->heart_rate;
    const float tbeat = fmodf((float)sim->t, T);
    const float tes   = 0.35f * T;
    const float tn    = tbeat / tes;
    const float tce   = params->RV_k0 + params->RV_k1 * T;

    /* LV elastance */

    const float e_min_LV =
        params->LV_a *
        (expf(params->LV_b * (state->V[CHAMBER_LV] - params->LV_c)) - 1.0f)
        + params->LV_d;

    const float d =
        params->LV_c2
        + params->LV_c3 * powf(tn, params->LV_beta)
        + params->LV_c4 * powf(tn, params->LV_gamma)
        + powf(tn, params->LV_delta);

    const float en =
        (params->LV_c1 * powf(tn, params->LV_alpha)) / d;

    params->e[CHAMBER_LV] =
        (params->e_LV_max - e_min_LV) * en + e_min_LV;

    /* RV elastance */

    const float e_min_RV =
        params->RV_a3 *
        (expf(params->RV_b3 * (state->V[CHAMBER_RV] - params->RV_c)) + 1.0f);

    float phi = 0.0f;

    if (tbeat < tce) {
        const float angle = CVS_PI * tbeat / tce;

        phi =
            params->RV_a_phi * sinf(angle)
            - params->RV_b_phi * sinf(2.0f * angle);
    }

    params->e[CHAMBER_RV] =
        e_min_RV + phi * (params->e_RV_max - e_min_RV);
}

// P = e * V
void cvs_calculate_pressures(CVS_Parameters *params, CVS_State *state)
{
    for (int i = 0; i < CHAMBER_COUNT; ++i) {
        state->P[i] = params->e[i] * (state->V[i] - params->V0[i]);
    }
}

// Kirchoff -> (P_next - P_current) * g
// Electrical Analogy this is (V_next - V_current) / R
void cvs_calculate_flows(CVS_Parameters *params, CVS_State *state)
{
    for (int i = 0; i < CHAMBER_COUNT; ++i) {
        const Chamber c = (Chamber)i;
        const Chamber next = cvs_next_chamber(c);

        float pressure_diff = state->P[c] - state->P[next];

        if (cvs_is_valve_outlet(c)) {
            pressure_diff = fmaxf(0.0f, pressure_diff);
        }

        state->Q[c] = params->g[c] * pressure_diff;
    }
}

void cvs_calculate_volumes(CVS_Simulation *sim, CVS_State *state)
{
    float V_dot[CHAMBER_COUNT];

    for (int i = 0; i < CHAMBER_COUNT; ++i) {
        const Chamber c = (Chamber)i;
        const Chamber prev = cvs_prev_chamber(c);

        // Calculate Volume Changes
        V_dot[c] = state->Q[prev] - state->Q[c];

        // Calculate Volumes using Euler Integration
        state->V[c] += V_dot[c] * (float)sim->dt;
    }
}

void cvs_write_output_header(CVS_Simulation *sim)
{
    fprintf(sim->output_file, "t");

    for (int i = 0; i < CHAMBER_COUNT; ++i) {
        fprintf(sim->output_file, ",V_%s", CHAMBER_NAMES[i]);
    }

    for (int i = 0; i < CHAMBER_COUNT; ++i) {
        fprintf(sim->output_file, ",P_%s", CHAMBER_NAMES[i]);
    }

    for (int i = 0; i < CHAMBER_COUNT; ++i) {
        fprintf(sim->output_file, ",Q_%s", CHAMBER_NAMES[i]);
    }

    fprintf(sim->output_file, "\n");
}

void cvs_write_output(CVS_Simulation *sim, CVS_State *state)
{
    fprintf(sim->output_file, "%.9f", sim->t);

    for (int i = 0; i < CHAMBER_COUNT; ++i) {
        fprintf(sim->output_file, ",%.9f", (double)state->V[i]);
    }

    for (int i = 0; i < CHAMBER_COUNT; ++i) {
        fprintf(sim->output_file, ",%.9f", (double)state->P[i]);
    }

    for (int i = 0; i < CHAMBER_COUNT; ++i) {
        fprintf(sim->output_file, ",%.9f", (double)state->Q[i]);
    }

    fprintf(sim->output_file, "\n");
}
