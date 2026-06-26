#include "cvs_volumes.h"

#define CVS_TOTAL_BLOOD_VOLUME 5000.0f

static CVS_Node cvs_previous_flow_node(CVS_Node node)
{
    /*
     * The Simulink Unit Delay stores volumes in the circulation loop.
     * For each node, the incoming flow is the outlet flow of the previous
     * node in this same loop:
     *
     *   RA -> RV -> P -> LA -> LV -> AO -> V -> RA
     */
    switch (node)
    {
    case NODE_RA:
        return NODE_V;
    case NODE_RV:
        return NODE_RA;
    case NODE_P:
        return NODE_RV;
    case NODE_LA:
        return NODE_P;
    case NODE_LV:
        return NODE_LA;
    case NODE_AO:
        return NODE_LV;
    case NODE_V:
    case NODE_COUNT:
    default:
        return NODE_AO;
    }
}

void cvs_initialize_volume_defaults(CVS_States *states)
{
    /*
     * Reference_CVS Unit Delay2 uses X_0 = [0, 0, 5000, 0, 0, 0, 0].
     * In the Simulink vector order this is [LV, AO, V, RV, P, LA, RA],
     * so only the venous compartment starts with blood volume.
     */
    for (int i = 0; i < NODE_COUNT; ++i)
    {
        states->V[i] = 0.0f;
    }

    states->V[NODE_V] = CVS_TOTAL_BLOOD_VOLUME;
}

void cvs_calculate_volumes(CVS_States *states, CVS_Simulation sim)
{
    float v_dot[NODE_COUNT];

    /*
     * Simulink computes the next Unit Delay input as:
     *
     *   V_next = V_current + stepsize * (Q_in - Q_out)
     *
     * The temporary v_dot array keeps the update simultaneous for all
     * nodes, matching the vector update in the reference model.
     */
    for (int i = 0; i < NODE_COUNT; ++i)
    {
        CVS_Node node = (CVS_Node)i;
        CVS_Node previous = cvs_previous_flow_node(node);

        v_dot[node] = states->Q[previous] - states->Q[node];
    }

    for (int i = 0; i < NODE_COUNT; ++i)
    {
        states->V[i] += v_dot[i] * (float)sim.dt;
    }

    /*
     * The Simulink vector equation conserves total volume because every
     * flow appears once as an outlet and once as an inlet. With float C
     * arithmetic a tiny roundoff residue accumulates over many steps, so
     * return that residue to the venous reservoir.
     */
    float total_volume = 0.0f;

    for (int i = 0; i < NODE_COUNT; ++i)
    {
        total_volume += states->V[i];
    }

    states->V[NODE_V] += CVS_TOTAL_BLOOD_VOLUME - total_volume;
}
