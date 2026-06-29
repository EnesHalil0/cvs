#ifndef CVS_SIM_ENGINE_H
#define CVS_SIM_ENGINE_H

// Simulation and Model Constants
#define DT 0.01         
#define SIM_STEPS 500
#define R_RESISTANCE 1.0  
#define C_COMPLIANCE 2.5  

// Solver Configuration Types
typedef enum {
    SOLVER_EULER,
    SOLVER_RK4
} SolverType;

// Function Prototypes
double cvs_derivative(double t, double P);

void run_cvs_simulation(SolverType active_solver, long long steps);

#endif // CVS_SIM_ENGINE_H