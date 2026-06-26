#include "cvs_elastances.h"
#include <math.h>

#define CVS_PI 3.14159265358979323846f

static float cvs_square(float value)
{
    return value * value;
}

static float cvs_calculate_e_AO(CVS_Parameters params)
{
    float hr = params.heart_rate;
    float hr2 = cvs_square(hr);

    /*
     * Simulink eAO switch order:
     *   exercise      -> Subsystem15
     *   else HF       -> hfao1
     *   else resting  -> eAO resting1
     */
    if (params.exercise)
    {
        return ((-0.0278f * hr2) + (5.8333f * hr) - 150.0f) * (1.0f / 100.0f);
    }

    if (params.heart_failure)
    {
        return (0.000003f * hr2) - (0.0008f * hr) + 0.5621f;
    }

    return ((0.5f * hr) + 40.0f) * (1.0f / 100.0f);
}

static float cvs_calculate_e_V(CVS_Parameters params)
{
    float hr = params.heart_rate;
    float hr2 = cvs_square(hr);

    if (params.heart_failure)
    {
        return ((-1.0e-16f * hr2) + (0.6667f * hr) - 3.0e-13f) * (1.0f / 10000.0f);
    }

    return ((-0.0033f * hr2) + (0.8667f * hr) - 16.0f) * (1.0f / 10000.0f);
}

static float cvs_calculate_e_P(CVS_Parameters params)
{
    float hr = params.heart_rate;
    float hr2 = cvs_square(hr);

    if (params.heart_failure)
    {
        return ((0.0167f * hr2) - (3.1667f * hr) + 240.0f) * (1.0f / 1000.0f);
    }

    return ((0.1417f * hr2) - (24.583f * hr) + 2165.0f) * (1.0f / 10000.0f);
}

static float cvs_calculate_e_LA(CVS_Parameters params)
{
    float hr = params.heart_rate;
    float hr2 = cvs_square(hr);

    if (params.heart_failure)
    {
        return ((0.0167f * hr2) - (2.8333f * hr) + 220.0f) * (1.0f / 1000.0f);
    }

    return ((-0.0069f * hr2) + (1.2813f * hr) + 30.375f) * (1.0f / 1000.0f);
}

static float cvs_calculate_e_RA(CVS_Parameters params)
{
    float hr = params.heart_rate;
    float hr2 = cvs_square(hr);

    if (params.heart_failure)
    {
        return ((0.015f * hr2) - (2.25f * hr) + 301.0f) * (1.0f / 1000.0f);
    }

    return ((-0.1479f * hr2) + (38.021f * hr) - 923.75f) * (1.0f / 10000.0f);
}

static void cvs_calculate_passive_elastances(CVS_Parameters *params)
{
    /*
     * These nodes are not beat-varying, but the Simulink reference still
     * recalculates them from HR and condition switches. LV/RV are updated
     * by their time-varying functions below.
     */
    params->e[NODE_RA] = cvs_calculate_e_RA(*params);
    params->e[NODE_P] = cvs_calculate_e_P(*params);
    params->e[NODE_LA] = cvs_calculate_e_LA(*params);
    params->e[NODE_AO] = cvs_calculate_e_AO(*params);
    params->e[NODE_V] = cvs_calculate_e_V(*params);
}

static float cvs_e_LV_min(CVS_Parameters params, CVS_States states)
{
    float hr = params.heart_rate;
    float hr2 = cvs_square(hr);

    float b_LV;
    float c_LV;

    if (params.heart_failure)
    {
        b_LV = ((-0.0333f * hr2) + (4.333f * hr) - 20.0f) * (1.0f / 10000.0f);
        c_LV = ((2.444f * hr2) - (306.67f * hr) + 25600.0f) * (1.0f / 100.0f);
    }
    else
    {
        if (params.exercise)
        {
            b_LV = ((0.0108f * hr2) - (4.2917f * hr) + 398.5f) * (1.0f / 10000.0f);
        }
        else
        {
            b_LV = ((-4.3333f * hr) + 440.0f) * (1.0f / 10000.0f);
        }

        c_LV = (0.0446f * hr2) - (6.3542f * hr) + 365.75f;
    }

    float a_LV = 0.4641f;
    float d_LV = 0.1235f;
    float u = (states.V[NODE_LV] - c_LV) * b_LV;

    return (a_LV * (expf(u) - 1.0f)) + d_LV;
}

static float cvs_e_LV_max(CVS_Parameters params)
{
    float hr = params.heart_rate;
    float hr2 = cvs_square(hr);

    if (params.heart_failure)
    {
        return ((-0.0028f * hr2) + (0.9167f * hr) + 45.0f) * (1.0f / 100.0f);
    }

    if (params.exercise)
    {
        return ((0.0027f * hr2) - (0.0767f * hr) + 24.76f) * (1.0f / 10.0f);
    }

    return 3.0f;
}

static float cvs_e_RV_min(CVS_Parameters params, CVS_States states)
{
    float e_RV_min = 0.0f;

    float a_RV = 0.03752f;
    float b_RV = 0.03776f;
    float c_RV;

    float hr = params.heart_rate;
    float hr2 = cvs_square(hr);

    // calculate c_RV according to heart failure condition
    if (params.heart_failure)
    {
        c_RV = ((hr * -333.33f) + 110000.0f) * (1.0f / 1000.0f);
    }
    else
    {
        c_RV = (0.0167f * hr2) - (3.3333f * hr) + 290.0f;
    }

    float u = (states.V[NODE_RV] - c_RV) * b_RV;

    e_RV_min = a_RV * (expf(u) + 1);

    return e_RV_min;
}

static float cvs_e_RV_max(CVS_Parameters params)
{
    float e_RV_max = 0.0f;

    float hr = params.heart_rate;
    float hr2 = cvs_square(hr);

    if (params.heart_failure)
    {
        e_RV_max = ((0.0056f * hr2) - (0.6667f * hr) + 60.0f) * (1.0f / 100.0f);
    }
    else
    {
        e_RV_max = ((0.005f * hr2) + (0.25f * hr) + 17.0f) * (1.0f / 100.0f);
    }

    return e_RV_max;
}

static float calculate_k_0(float hr)
{
    float k_0;

    float hr2 = cvs_square(hr);

    k_0 = ((hr2 * -0.0041f) + (-0.0417f * hr) + 257.5f) * (1.0f / 1000.0f);

    return k_0;
}

static float calculate_k_1(float hr)
{
    float k_1;
    float hr2 = cvs_square(hr);

    k_1 = ((hr2 * 0.0158f) + (-5.0417f * hr) + 445.5f) * (1.0f / 1000.0f);

    return k_1;
}

static float cvs_calculate_normalized_e_LV(float t_beat, float t_es)
{
    if (t_es <= 0.0f)
    {
        return 0.0f;
    }

    float t_n = t_beat / t_es;
    float t_n_alpha = powf(t_n, 1.5f);
    float t_n_beta = powf(t_n, 1.9f);
    float t_n_gamma = powf(t_n, 37.0f);
    float t_n_delta = powf(t_n, 38.9f);

    float numerator = 461.4567f * t_n_alpha;
    float denominator = 126.2886f + (333.3319f * t_n_beta) + (0.3789f * t_n_gamma) + t_n_delta;

    if (denominator == 0.0f)
    {
        return 0.0f;
    }

    return numerator / denominator;
}

static bool cvs_is_RV_active(float t_beat, float t_ce)
{
    if (t_ce <= 0.0f)
    {
        return false;
    }

    return (t_beat >= 0.0f) && (t_beat < t_ce);
}

static float cvs_calculate_phi_RV(float t_beat, float t_ce)
{
    if (!cvs_is_RV_active(t_beat, t_ce))
    {
        return 0.0f;
    }

    float x = CVS_PI * t_beat / t_ce;

    return 0.9f * sinf(x) - 0.25f * sinf(2.0f * x);
}

void cvs_calculate_e_LV(CVS_Parameters *params, CVS_States states, CVS_Simulation sim)
{
    float hr = params->heart_rate;
    float e_LV_min = cvs_e_LV_min(*params, states);
    float e_LV_max = cvs_e_LV_max(*params);

    float freq = hr / 60.0f;
    float T = 1.0f / freq;
    float t_beat = fmodf(sim.t, T);
    float t_es = 0.35f / freq;
    float e_n = cvs_calculate_normalized_e_LV(t_beat, t_es);

    params->e[NODE_LV] = (e_n * (e_LV_max - e_LV_min)) + e_LV_min;
}

void cvs_calculate_e_RV(CVS_Parameters *params, CVS_States states, CVS_Simulation sim)
{
    float e_RV = 0.0f;
    float hr = params->heart_rate;

    float e_RV_min = cvs_e_RV_min(*params, states);
    float e_RV_max = cvs_e_RV_max(*params);

    float freq = hr / 60.0f;
    float T = 1.0f / freq;
    float t_beat = fmodf(sim.t, T); // time in beat
    // float t_es = 0.35 / freq;         // time end systolic
    // float t_n = t_beat / t_es;
    float t_h = 1.0f / freq;

    float k_0 = calculate_k_0(hr);
    float k_1 = calculate_k_1(hr);
    float t_ce = k_0 + (k_1 * t_h);

    float phi = cvs_calculate_phi_RV(t_beat, t_ce);

    e_RV = (e_RV_max * phi) + (e_RV_min * (1 - phi));

    params->e[NODE_RV] = e_RV;
}

void cvs_calculate_elastances(CVS_Parameters *params, CVS_States states, CVS_Simulation sim)
{
    cvs_calculate_passive_elastances(params);
    cvs_calculate_e_LV(params, states, sim);
    cvs_calculate_e_RV(params, states, sim);
}
