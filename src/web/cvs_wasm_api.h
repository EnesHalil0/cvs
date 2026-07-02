#ifndef CVS_WASM_API_H
#define CVS_WASM_API_H

void cvs_wasm_init(float heart_rate, double dt, double duration);
void cvs_wasm_init_ex(float heart_rate, double dt, double duration, int heart_failure, int exercise);
void cvs_wasm_init_solver(float heart_rate,
                          double dt,
                          double duration,
                          int heart_failure,
                          int exercise,
                          int solver_type);
void cvs_wasm_step(void);
void cvs_wasm_run_steps(int step_count);
double cvs_wasm_get_time(void);

float cvs_wasm_get_volume(int node);
float cvs_wasm_get_pressure(int node);
float cvs_wasm_get_flow(int node);
float cvs_wasm_get_e_lv(void);
float cvs_wasm_get_e_rv(void);

#endif // CVS_WASM_API_H
