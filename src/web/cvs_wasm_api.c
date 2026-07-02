#include "cvs_wasm_api.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define CVS_WASM_KEEPALIVE EMSCRIPTEN_KEEPALIVE
#else
#define CVS_WASM_KEEPALIVE
#endif

#include "cvs_core.h"

static CVS_Model cvs_wasm_model;

static int cvs_wasm_is_valid_node(int node)
{
    return node >= 0 && node < NODE_COUNT;
}

CVS_WASM_KEEPALIVE
void cvs_wasm_init(float heart_rate, double dt, double duration)
{
    cvs_wasm_init_ex(heart_rate, dt, duration, 0, 0);
}

CVS_WASM_KEEPALIVE
void cvs_wasm_init_ex(float heart_rate, double dt, double duration, int heart_failure, int exercise)
{
    cvs_wasm_init_solver(heart_rate, dt, duration, heart_failure, exercise, CVS_SOLVER_EULER);
}

CVS_WASM_KEEPALIVE
void cvs_wasm_init_solver(float heart_rate,
                          double dt,
                          double duration,
                          int heart_failure,
                          int exercise,
                          int solver_type)
{
    CVS_ModelConfig config = {
        .heart_rate = heart_rate,
        .dt = (float)dt,
        .duration = (float)duration,
        .heart_failure = heart_failure != 0,
        .exercise = exercise != 0,
        .solver_type = (CVS_SolverType)solver_type,
    };

    cvs_model_init(&cvs_wasm_model, &config);
}

CVS_WASM_KEEPALIVE
void cvs_wasm_step(void)
{
    cvs_model_step(&cvs_wasm_model);
}

CVS_WASM_KEEPALIVE
void cvs_wasm_run_steps(int step_count)
{
    for (int i = 0; i < step_count; ++i)
    {
        cvs_model_step(&cvs_wasm_model);
    }
}

CVS_WASM_KEEPALIVE
double cvs_wasm_get_time(void)
{
    return (double)cvs_model_get_simulation(&cvs_wasm_model).t;
}

CVS_WASM_KEEPALIVE
float cvs_wasm_get_volume(int node)
{
    const CVS_States *states = cvs_model_get_state(&cvs_wasm_model);

    if (!cvs_wasm_is_valid_node(node))
    {
        return 0.0f;
    }

    return states->V[node];
}

CVS_WASM_KEEPALIVE
float cvs_wasm_get_pressure(int node)
{
    const CVS_States *states = cvs_model_get_state(&cvs_wasm_model);

    if (!cvs_wasm_is_valid_node(node))
    {
        return 0.0f;
    }

    return states->P[node];
}

CVS_WASM_KEEPALIVE
float cvs_wasm_get_flow(int node)
{
    const CVS_States *states = cvs_model_get_state(&cvs_wasm_model);

    if (!cvs_wasm_is_valid_node(node))
    {
        return 0.0f;
    }

    return states->Q[node];
}

CVS_WASM_KEEPALIVE
float cvs_wasm_get_e_lv(void)
{
    const CVS_Parameters *params = cvs_model_get_params(&cvs_wasm_model);

    return params->e[NODE_LV];
}

CVS_WASM_KEEPALIVE
float cvs_wasm_get_e_rv(void)
{
    const CVS_Parameters *params = cvs_model_get_params(&cvs_wasm_model);

    return params->e[NODE_RV];
}
