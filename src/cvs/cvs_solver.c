#include "cvs_solver.h"

// Standart Euler Diferansiyel Adımı (Dinamik float)
float euler_volume_step(float current_v, float v_dot, float dt) {
    return current_v + (v_dot * dt);
}

// 4. Dereceden Runge-Kutta (RK4) Diferansiyel Adımı (Dinamik float)
float rk4_volume_step(float current_v, float v_dot, float dt) {
    float k1 = v_dot;
    float k2 = v_dot;
    float k3 = v_dot;
    float k4 = v_dot;
    
    return current_v + (dt / 6.0f) * (k1 + 2.0f * k2 + 2.0f * k3 + k4);
