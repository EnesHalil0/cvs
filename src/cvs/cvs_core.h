#ifndef CVS_CORE_H
#define CVS_CORE_H

#include <stdbool.h>

#include "cvs_model.h"
#include "cvs_solver.h"

typedef struct
{
    float heart_rate;
    float dt;
    float duration;
    bool heart_failure;
    bool exercise;
    CVS_SolverType solver_type;
} CVS_ModelConfig;

typedef struct
{
    CVS_Parameters params;
    CVS_States states;
    CVS_Simulation simulation;
    CVS_Solver solver;
    int step_index;
} CVS_Model;

void cvs_model_init(CVS_Model *model, const CVS_ModelConfig *config);
void cvs_model_step(CVS_Model *model);

const CVS_States *cvs_model_get_state(const CVS_Model *model);
const CVS_Parameters *cvs_model_get_params(const CVS_Model *model);
CVS_Simulation cvs_model_get_simulation(const CVS_Model *model);

#endif // CVS_CORE_H
