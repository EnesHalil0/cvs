#include "cvs_elastances.h"
#include <math.h>

static float cvs_e_RV_min(const CVS_Model m)
{
    float e_RV_min = 0;

    float a_RV = 0.03752;
    float b_RV = 0.03776;
    float c_RV;

    float hr = m.heart_rate;
    float hr2 = hr * hr;

    // calculate c_RV according to heart failure condition
    if (m.c.heart_failure)
    {
        c_RV = ((hr * -333.33) + 110000) * (1 / 1000);
    }
    else
    {
        c_RV = (0.0167 * hr2) - (3.3333 * hr) + 290;
    }

    float u = (m.s.V.RV - c_RV) * b_RV;

    e_RV_min = a_RV * (expf(u) + 1);

    return e_RV_min;
}

static float cvs_e_RV_max(const CVS_Model m)
{
    float e_RV_max = 0.0f;

    float hr = m.heart_rate;
    float hr2 = hr * hr;

    if (m.c.heart_failure)
    {
        e_RV_max = ((0.0056 * hr2) - (0.6667 * hr) + 60) * (1 / 100);
    }
    else
    {
        e_RV_max = ((0.005 * hr2) + (0.25 * hr) + 17) * (1 / 100);
    }

    return e_RV_max;
}

static float calculate_k_0(float hr)
{
    float k_0;

    float hr2 = hr * hr;

    k_0 = (hr2 * -0.0041) + (-0.0417 * hr) + 257.5;

    return k_0;
}

static float calculate_k_1(float hr)
{
    float k_1;

    return k_1;
}

void cvs_calculate_e_RV(CVS_Model *m)
{
    float e_RV = 0.0f;
    float hr = m->heart_rate;

    float e_RV_min = cvs_e_RV_min(*m);
    float e_RV_max = cvs_e_RV_max(*m);

    float freq = hr / 60;
    float T = 1 / freq;
    float t_beat = fmodf(m->time, T); // time in beat
    float t_es = 0.35 / freq;         // time end systolic
    float t_n = t_beat / t_es;
    float t_h = 1 / freq;

    float k_0 = calculate_k_0(hr);
    float k_1 = calculate_k_1(hr);
    float t_ce = k_0 + (k_1 * t_h);

    float phi = 0.0f;

    phi = () * ();

    e_RV = (e_RV_max * phi) + (e_RV_min * (1 - phi));
}