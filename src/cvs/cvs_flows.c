#include "cvs_flows.h"

#include <math.h>
#include <stdbool.h>

static CVS_Node cvs_next_flow_node(CVS_Node node)
{
    /*
     * The CVS loop follows the same circulation order used by the
     * Simulink flow vector:
     *
     *   RA -> RV -> P -> LA -> LV -> AO -> V -> RA
     *
     * Q[node] is therefore the outlet flow from node to this next node.
     */
    switch (node)
    {
    case NODE_RA:
        return NODE_RV;
    case NODE_RV:
        return NODE_P;
    case NODE_P:
        return NODE_LA;
    case NODE_LA:
        return NODE_LV;
    case NODE_LV:
        return NODE_AO;
    case NODE_AO:
        return NODE_V;
    case NODE_V:
    case NODE_COUNT:
    default:
        return NODE_RA;
    }
}

static bool cvs_is_valve_outlet(CVS_Node node)
{
    /*
     * Simulink clamps the four valve pressure drops with Max(0, dP):
     * RA->RV, RV->P, LA->LV, and LV->AO. Systemic and pulmonary
     * vascular paths are not clamped, so reverse pressure differences
     * are allowed there.
     */
    return (node == NODE_RA) ||
           (node == NODE_RV) ||
           (node == NODE_LA) ||
           (node == NODE_LV);
}

static float cvs_square(float value)
{
    return value * value;
}

static float cvs_calculate_g_AO(const CVS_Parameters *params)
{
    float hr = params->heart_rate;
    float hr2 = cvs_square(hr);

    /*
     * Simulink switch order:
     *   heart_failure -> hf11
     *   else exercise -> Subsystem20
     *   else          -> gao resting1
     */
    if (params->heart_failure)
    {
        return ((0.0079f * hr2) - (0.7958f * hr) + 117.5f) * (1.0f / 100.0f);
    }

    if (params->exercise)
    {
        return ((0.0058f * hr2) + (1.4583f * hr) - 18.5f) * (1.0f / 100.0f);
    }

    return ((-0.0167f * hr2) + (3.25f * hr) - 45.0f) * (1.0f / 100.0f);
}

static float cvs_calculate_g_RV(const CVS_Parameters *params)
{
    float hr = params->heart_rate;
    float hr2 = cvs_square(hr);

    if (params->heart_failure)
    {
        return (0.0222f * hr2) - (4.3333f * hr) + 230.0f;
    }

    return (0.0742f * hr2) - (10.958f * hr) + 600.5f;
}

static void cvs_update_flow_conductances(CVS_Parameters *params)
{
    float hr = params->heart_rate;
    float hr2 = cvs_square(hr);

    /*
     * Conductance values come from the Simulink "g Values" subsystems.
     * They are recalculated from HR each step so future HR/HF/exercise
     * changes are reflected before Q is computed.
     */
    params->g[NODE_RA] = (-2.5f * hr) + 1150.0f;
    params->g[NODE_RV] = cvs_calculate_g_RV(params);
    params->g[NODE_P] = ((-0.0208f * hr2) + (4.7917f * hr) - 162.5f) * (1.0f / 10.0f);
    params->g[NODE_LA] = (-5.0f * hr) + 1300.0f;
    params->g[NODE_LV] = ((4.1667f * hr2) + (3041.7f * hr) + 12500.0f) * (1.0f / 1000.0f);
    params->g[NODE_AO] = cvs_calculate_g_AO(params);
    params->g[NODE_V] = ((0.25f * hr2) + (1212.5f * hr) - 3650.0f) * (1.0f / 1000.0f);
}

void cvs_initialize_flow_defaults(CVS_Parameters *params)
{
    cvs_update_flow_conductances(params);
}

void cvs_calculate_flows(CVS_Parameters *params, CVS_States *states)
{
    cvs_update_flow_conductances(params);

    for (int i = 0; i < NODE_COUNT; ++i)
    {
        CVS_Node node = (CVS_Node)i;
        CVS_Node next = cvs_next_flow_node(node);
        float pressure_diff = states->P[node] - states->P[next];

        if (cvs_is_valve_outlet(node))
        {
            pressure_diff = fmaxf(0.0f, pressure_diff);
        }

        states->Q[node] = params->g[node] * pressure_diff;
    }
}
