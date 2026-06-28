#define _USE_MATH_DEFINES  // Math constants like M_PI
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "cvs_solver.h"

/*
 * @brief Derivative function for the 2-Element Windkessel Model (dP/dt)
 * Formula: dP/dt = (Q_in / C) - (P / (R * C))
 */
double cvs_derivative(double t, double P) {
    double heart_rate = 60.0; // Heart rate in BPM
    double cardiac_cycle = 60.0 / heart_rate; // Duration of one cardiac cycle (s)
    double systole_duration = 0.3 * cardiac_cycle; // Systole duration (30% of cycle)
    double Q_in = 0.0;

    // Compute current time position within the cardiac cycle 
    double time_in_cycle = fmod(t, cardiac_cycle);

    // Simulink Reference: Pulse generator modeling a half-sine wave
    if (time_in_cycle < systole_duration) {
        // Systole: Active dynamic blood flow 
        Q_in = 50.0 * sin(M_PI * time_in_cycle / systole_duration);
    } else {
        // Diastole: Aortic valve closed, zero flow 
        Q_in = 0.0;
    }

    // Evaluation of the differential equation
    return (Q_in / C_COMPLIANCE) - (P / (R_RESISTANCE * C_COMPLIANCE));
}

int main() {
    // -----------------------------------------------------------------
    // SOLVER CONFIGURATION: Must match the Simulink Fixed-step configuration.
    // -----------------------------------------------------------------
    SolverType active_solver = SOLVER_RK4; // Maps to Simulink ode4 (Runge-Kutta)
    //SolverType active_solver = SOLVER_EULER; // Maps to Simulink ode1 (Euler) 
    // -----------------------------------------------------------------

    // Initialize the profiling clock 
    clock_t start_time = clock();

    // Open CSV file for data logging 
    FILE *fp;
    if (active_solver == SOLVER_EULER) {
        fp = fopen("euler_results.csv", "w");
        printf("Euler (ODE1) Solver running...\n");
    } else {
        fp = fopen("rk4_results.csv", "w");
        printf("Runge-Kutta 4 (ODE4) Solver running...\n");
    }

    if (fp == NULL) {
        printf("File opening error!\n");
        return 1;
    }

    // Print CSV header compatible with MATLAB
    fprintf(fp, "Time,Pressure\n");

    // Initial boundary conditions
    double t = 0.0;
    double P = 80.0; // Initial diastolic pressure (mmHg)

    // SIMULATION LOOP (Continuous-to-Discrete State Update)
    for (long long i = 0; i < SIM_STEPS; i++) {
        //fprintf(fp, "%.3f,%.3f\n", t, P);

        if (active_solver == SOLVER_EULER) {
            // --- EULER METHOD (ODE1) ---
            double dP = cvs_derivative(t, P);
            P = P + DT * dP;
        } 
        else if (active_solver == SOLVER_RK4) {
            // --- RUNGE-KUTTA 4 (ODE4) ---
            double k1 = cvs_derivative(t, P);
            double k2 = cvs_derivative(t + DT/2.0, P + (DT/2.0) * k1);
            double k3 = cvs_derivative(t + DT/2.0, P + (DT/2.0) * k2);
            double k4 = cvs_derivative(t + DT, P + DT * k3);
            
            // Apply weighted average step update
            P = P + (DT / 6.0) * (k1 + 2.0*k2 + 2.0*k3 + k4);
        }

        t += DT; //Advance discrete simulation time step
    }

    fclose(fp);

    // Stop profiling clock and compute elapsed CPU time
    clock_t end_time = clock();
    
    double cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Simulation completed. Results saved to file.\n");

    printf("Final calculated Pressure: %f mmHg\n", P);

    printf("\n====================================\n");
    printf("Simulation total execution time: %f seconds\n", cpu_time_used);
    printf("====================================\n");

    return 0;
}