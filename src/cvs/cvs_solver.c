#include "cvs_solver.h"

static void cvs_solver_copy_state_with_volume_delta(CVS_States *trial_state,
                                                    const CVS_States *base_state,
                                                    const float dVdt[NODE_COUNT],
                                                    double scale)
{
    *trial_state = *base_state;

    for (int i = 0; i < NODE_COUNT; ++i)
    {
        trial_state->V[i] = base_state->V[i] + (float)(scale * dVdt[i]);
    }
}

static void cvs_solver_step_euler(void *context,
                                  CVS_States *states,
                                  double t,
                                  double dt,
                                  CVS_RhsFn rhs_fn)
{
    float k1[NODE_COUNT];

    rhs_fn(context, states, t, k1);

    for (int i = 0; i < NODE_COUNT; ++i)
    {
        states->V[i] += (float)(dt * k1[i]);
    }
}

static void cvs_solver_step_rk2_heun(void *context,
                                     CVS_States *states,
                                     double t,
                                     double dt,
                                     CVS_RhsFn rhs_fn)
{
    CVS_States original_state = *states;
    CVS_States trial_state;
    float k1[NODE_COUNT];
    float k2[NODE_COUNT];

    rhs_fn(context, &original_state, t, k1);
    cvs_solver_copy_state_with_volume_delta(&trial_state, &original_state, k1, dt);
    rhs_fn(context, &trial_state, t + dt, k2);

    for (int i = 0; i < NODE_COUNT; ++i)
    {
        states->V[i] = original_state.V[i] + (float)(dt * 0.5 * (k1[i] + k2[i]));
    }
}

static void cvs_solver_step_rk2_midpoint(void *context,
                                         CVS_States *states,
                                         double t,
                                         double dt,
                                         CVS_RhsFn rhs_fn)
{
    CVS_States original_state = *states;
    CVS_States trial_state;
    float k1[NODE_COUNT];
    float k2[NODE_COUNT];

    rhs_fn(context, &original_state, t, k1);
    cvs_solver_copy_state_with_volume_delta(&trial_state, &original_state, k1, 0.5 * dt);
    rhs_fn(context, &trial_state, t + 0.5 * dt, k2);

    for (int i = 0; i < NODE_COUNT; ++i)
    {
        states->V[i] = original_state.V[i] + (float)(dt * k2[i]);
    }
}

static void cvs_solver_step_rk4(void *context,
                                CVS_States *states,
                                double t,
                                double dt,
                                CVS_RhsFn rhs_fn)
{
    CVS_States original_state = *states;
    CVS_States trial_state;
    float k1[NODE_COUNT];
    float k2[NODE_COUNT];
    float k3[NODE_COUNT];
    float k4[NODE_COUNT];

    rhs_fn(context, &original_state, t, k1);
    cvs_solver_copy_state_with_volume_delta(&trial_state, &original_state, k1, 0.5 * dt);
    rhs_fn(context, &trial_state, t + 0.5 * dt, k2);
    cvs_solver_copy_state_with_volume_delta(&trial_state, &original_state, k2, 0.5 * dt);
    rhs_fn(context, &trial_state, t + 0.5 * dt, k3);
    cvs_solver_copy_state_with_volume_delta(&trial_state, &original_state, k3, dt);
    rhs_fn(context, &trial_state, t + dt, k4);

    for (int i = 0; i < NODE_COUNT; ++i)
    {
        states->V[i] = original_state.V[i] +
                       (float)((dt / 6.0) * (k1[i] + 2.0f * k2[i] + 2.0f * k3[i] + k4[i]));
    }
}

void cvs_solver_init(CVS_Solver *solver, CVS_SolverType type)
{
    solver->type = type;

    switch (type)
    {
    case CVS_SOLVER_EULER:
        solver->type = CVS_SOLVER_EULER;
        solver->step = cvs_solver_step_euler;
        break;
    case CVS_SOLVER_RK2_HEUN:
        solver->type = CVS_SOLVER_RK2_HEUN;
        solver->step = cvs_solver_step_rk2_heun;
        break;
    case CVS_SOLVER_RK2_MIDPOINT:
        solver->type = CVS_SOLVER_RK2_MIDPOINT;
        solver->step = cvs_solver_step_rk2_midpoint;
        break;
    case CVS_SOLVER_RK4:
        solver->type = CVS_SOLVER_RK4;
        solver->step = cvs_solver_step_rk4;
        break;
    default:
        solver->type = CVS_SOLVER_EULER;
        solver->step = cvs_solver_step_euler;
        break;
    }
}

void cvs_solver_step(const CVS_Solver *solver,
                     void *context,
                     CVS_States *states,
                     double t,
                     double dt,
                     CVS_RhsFn rhs_fn)
{
    solver->step(context, states, t, dt, rhs_fn);
}
