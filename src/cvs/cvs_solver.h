#ifndef CVS_SOLVER_H
#define CVS_SOLVER_H

#include "cvs_model.h"

// Solver fonksiyonları için ortak Function Pointer tipi
typedef float (*CVS_SolverFunc)(float current_v, float v_dot, float dt);

// Gelişmiş matematik motorları
float euler_volume_step(float current_v, float v_dot, float dt);
float rk4_volume_step(float current_v, float v_dot, float dt);

#endif // CVS_SOLVER_H
