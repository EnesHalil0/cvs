#ifndef CVS_MODEL_H
#define CVS_MODEL_H

#include <stdio.h>

typedef enum {
    CHAMBER_RA,   // Right Atrium
    CHAMBER_RV,   // Right Ventricle
    CHAMBER_P,    // Pulmonary circulation
    CHAMBER_LA,   // Left Atrium
    CHAMBER_LV,   // Left Ventricle
    CHAMBER_AO,   // Aorta
    CHAMBER_V,    // Veins

    CHAMBER_COUNT // Number of chamber states
} Chamber;

typedef struct {
    float heart_rate; // Heart rate [bpm]

    float e_LV_max;   // Maximum LV elastance
    float e_RV_max;   // Maximum RV elastance

    float V0[CHAMBER_COUNT]; // zero-pressure volumes
    
    float e[CHAMBER_COUNT]; // Chamber elastances: e[i] belongs to chamber i
    float g[CHAMBER_COUNT]; // Outlet admittances: g[i] is flow path i -> next(i)

    // Left ventricle elastance model parameters
    float LV_a;
    float LV_b;
    float LV_c;
    float LV_d;
    float LV_c1;
    float LV_c2;
    float LV_c3;
    float LV_c4;
    float LV_beta;
    float LV_gamma;
    float LV_delta;
    float LV_alpha;

    // Right ventricle elastance model parameters
    float RV_c;
    float RV_k0;
    float RV_k1;
    float RV_a3;
    float RV_b3;
    float RV_a_phi;
    float RV_b_phi;
} CVS_Parameters;

typedef struct {
    float V[CHAMBER_COUNT]; // Chamber Volumes
    float P[CHAMBER_COUNT]; // Chamber Pressures
    float Q[CHAMBER_COUNT]; // Outlet flows: Q[i] is chamber i -> next(i)
} CVS_State;

typedef struct  {
    double t;  // Current simulation time
    double dt; // Time step
    double duration; // Total simulation duration
    
    const char *output_file_path; // Output filename
    FILE *output_file; // Output file pointer
} CVS_Simulation;

static const char *CHAMBER_NAMES[CHAMBER_COUNT] = {
    [CHAMBER_RA] = "RA" ,
    [CHAMBER_RV] = "RV" ,
    [CHAMBER_P]  = "P"  ,
    [CHAMBER_LA] = "LA" ,
    [CHAMBER_LV] = "LV" ,
    [CHAMBER_AO] = "AO" ,
    [CHAMBER_V]  = "V"  ,
};

static inline Chamber cvs_next_chamber(Chamber c)
{
    if (c >= CHAMBER_COUNT) {
        return CHAMBER_RA;
    }

    return (Chamber)((c + 1) % CHAMBER_COUNT);
}

static inline Chamber cvs_prev_chamber(Chamber c)
{
    if (c >= CHAMBER_COUNT) {
        return CHAMBER_RA;
    }

    return (Chamber)((c + CHAMBER_COUNT - 1) % CHAMBER_COUNT);
}

static inline bool cvs_is_valve_outlet(Chamber c)
{
    return c == CHAMBER_RA ||
           c == CHAMBER_RV ||
           c == CHAMBER_LA ||
           c == CHAMBER_LV;
}

void cvs_simulation_init      (CVS_Simulation *sim, CVS_Parameters *params, CVS_State *state);
void cvs_simulation_step      (CVS_Simulation *sim, CVS_Parameters *params, CVS_State *state);
void cvs_simulation_run       (CVS_Simulation *sim, CVS_Parameters *params, CVS_State *state);
void cvs_simulation_close     (CVS_Simulation *sim);
void cvs_calculate_elastances (CVS_Simulation *sim, CVS_Parameters *params, CVS_State *state);
void cvs_calculate_pressures  (CVS_Parameters *params, CVS_State *state);
void cvs_calculate_flows      (CVS_Parameters *params, CVS_State *state);
void cvs_calculate_volumes    (CVS_Simulation *sim, CVS_State *state);
void cvs_write_output_header  (CVS_Simulation *sim);
void cvs_write_output         (CVS_Simulation *sim, CVS_State *state);

#endif //  CVS_MODEL_H
