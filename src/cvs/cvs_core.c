#include "cvs_core.h"

#include "cvs_elastances.h"
#include "cvs_flows.h"
#include "cvs_pressures.h"
#include "cvs_volumes.h"

#define CVS_TOTAL_BLOOD_VOLUME 5000.0f

static CVS_Node cvs_model_previous_flow_node(CVS_Node node)
{
    switch (node)
    {
    case NODE_RA:
        return NODE_V;
    case NODE_RV:
        return NODE_RA;
    case NODE_P:
        return NODE_RV;
    case NODE_LA:
        return NODE_P;
    case NODE_LV:
        return NODE_LA;
    case NODE_AO:
        return NODE_LV;
    case NODE_V:
    case NODE_COUNT:
    default:
        return NODE_AO;
    }
}

static void cvs_model_rhs(void *context,
                          const CVS_States *trial_state,
                          double trial_time,
                          float dVdt[NODE_COUNT])
{
    CVS_Model *model = (CVS_Model *)context;
    CVS_Parameters trial_params = model->params;
    CVS_States trial_states = *trial_state;
    CVS_Simulation trial_sim = model->simulation;

    trial_sim.t = (float)trial_time;

    cvs_calculate_elastances(&trial_params, trial_states, trial_sim);
    cvs_calculate_pressures(&trial_params, &trial_states);
    cvs_calculate_flows(&trial_params, &trial_states);

    for (int i = 0; i < NODE_COUNT; ++i)
    {
        CVS_Node node = (CVS_Node)i;
        CVS_Node previous = cvs_model_previous_flow_node(node);

        dVdt[node] = trial_states.Q[previous] - trial_states.Q[node];
    }
}

static void cvs_model_conserve_total_volume(CVS_Model *model)
{
    float total_volume = 0.0f;

    for (int i = 0; i < NODE_COUNT; ++i)
    {
        total_volume += model->states.V[i];
    }

    model->states.V[NODE_V] += CVS_TOTAL_BLOOD_VOLUME - total_volume;
}

void cvs_model_init(CVS_Model *model, const CVS_ModelConfig *config)
{
    *model = (CVS_Model){0};

    model->simulation.t = 0.0f;
    model->simulation.dt = config->dt;
    model->simulation.duration = config->duration;

    model->params.heart_rate = config->heart_rate;
    model->params.heart_failure = config->heart_failure;
    model->params.exercise = config->exercise;

    cvs_solver_init(&model->solver, config->solver_type);
    cvs_initialize_volume_defaults(&model->states);
    cvs_initialize_pressure_defaults(&model->params);
    cvs_initialize_flow_defaults(&model->params);
}

void cvs_model_step(CVS_Model *model)
{
    model->step_index += 1;
    model->simulation.t = (float)model->step_index * model->simulation.dt;

    cvs_calculate_elastances(&model->params, model->states, model->simulation);
    cvs_calculate_pressures(&model->params, &model->states);
    cvs_calculate_flows(&model->params, &model->states);
    cvs_solver_step(&model->solver,
                    model,
                    &model->states,
                    model->simulation.t,
                    model->simulation.dt,
                    cvs_model_rhs);
    cvs_model_conserve_total_volume(model);

    if (model->solver.type != CVS_SOLVER_EULER)
    {
        cvs_calculate_elastances(&model->params, model->states, model->simulation);
        cvs_calculate_pressures(&model->params, &model->states);
        cvs_calculate_flows(&model->params, &model->states);
    }
}

const CVS_States *cvs_model_get_state(const CVS_Model *model)
{
    return &model->states;
}

const CVS_Parameters *cvs_model_get_params(const CVS_Model *model)
{
    return &model->params;
}

CVS_Simulation cvs_model_get_simulation(const CVS_Model *model)
{
    return model->simulation;
}
