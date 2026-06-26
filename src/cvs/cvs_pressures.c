#include "cvs_pressures.h"

static float cvs_calculate_ventricular_v0(const CVS_Parameters *params)
{
    float hr = params->heart_rate;
    float hr2 = hr * hr;

    /*
     * Reference_CVS Subsystem13 computes one ventricular V0 scalar.
     * The Switch control is Heart_Failure_On_Off with threshold 0:
     *
     *   control > 0  -> (0.1944*HR^2 - 28.333*HR + 1100) / 10
     *   control <= 0 -> (-0.4167*HR^2 + 145.83*HR + 2750) / 1000
     */
    if (params->heart_failure)
    {
        return ((0.1944f * hr2) - (28.333f * hr) + 1100.0f) * (1.0f / 10.0f);
    }

    return ((-0.4167f * hr2) + (145.83f * hr) + 2750.0f) * (1.0f / 1000.0f);
}

static void cvs_update_pressure_v0(CVS_Parameters *params)
{
    float ventricular_v0 = cvs_calculate_ventricular_v0(params);

    /*
     * Simulink exposes V0 as a scalar for ventricular pressure blocks.
     * Non-ventricular nodes are modeled as direct P=e*V in the current
     * pressure path, so their offset remains zero.
     */
    for (int i = 0; i < NODE_COUNT; ++i)
    {
        params->V0[i] = 0.0f;
    }

    params->V0[NODE_LV] = ventricular_v0;
    params->V0[NODE_RV] = ventricular_v0;
}

void cvs_initialize_pressure_defaults(CVS_Parameters *params)
{
    /*
     * Initialize V0 from the same HR/HF switch used by the Simulink
     * reference. It is refreshed again in cvs_calculate_pressures()
     * so parameter changes are reflected during a run.
     */
    cvs_update_pressure_v0(params);
}

void cvs_calculate_pressures(CVS_Parameters *params, CVS_States *states)
{
    /*
     * Lumped-parameter CVS pressure relation:
     *
     *     P_node = e_node * (V_node - V0_node)
     *
     * e_node is the node elastance, V_node is the current volume,
     * and V0_node is the unstressed/reference volume. LV and RV
     * elastances must already be updated for the current time step.
     */
    cvs_update_pressure_v0(params);

    for (int i = 0; i < NODE_COUNT; ++i)
    {
        states->P[i] = params->e[i] * (states->V[i] - params->V0[i]);
    }
}
