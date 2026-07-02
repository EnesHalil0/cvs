#ifndef CVS_SOLVER_H
#define CVS_SOLVER_H

#include "cvs_model.h"

typedef enum
{
    CVS_SOLVER_EULER = 0,
    CVS_SOLVER_RK2_HEUN,
    CVS_SOLVER_RK2_MIDPOINT,
    CVS_SOLVER_RK4,
} CVS_SolverType;

typedef void (*CVS_RhsFn)(void *context,
                          const CVS_States *trial_state,
                          double trial_time,
                          float dVdt[NODE_COUNT]);

typedef void (*CVS_SolverStepFn)(void *context,
                                 CVS_States *states,
                                 double t,
                                 double dt,
                                 CVS_RhsFn rhs_fn);

typedef struct
{
    CVS_SolverType type;
    CVS_SolverStepFn step;
} CVS_Solver;

void cvs_solver_init(CVS_Solver *solver, CVS_SolverType type);
void cvs_solver_step(const CVS_Solver *solver,
                     void *context,
                     CVS_States *states,
                     double t,
                     double dt,
                     CVS_RhsFn rhs_fn);

#endif // CVS_SOLVER_H
