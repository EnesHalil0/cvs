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

void cvs_calculate_e_RV(CVS_Model *m)
{
    float e_RV = 0.0f;
    float hr = m->heart_rate;

    float e_RV_min = cvs_e_RV_min(*m);
    float e_RV_max = cvs_e_RV_max(*m);

    float phi = 0.0f;

    phi = () * ();

    e_RV = (e_RV_max * phi) + (e_RV_min * (1 - phi));

}