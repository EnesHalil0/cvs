#include "cvs_output.h"

static const CVS_Node CVS_OUTPUT_NODE_ORDER[NODE_COUNT] = {
    NODE_RA,
    NODE_RV,
    NODE_P,
    NODE_LA,
    NODE_LV,
    NODE_AO,
    NODE_V,
};

static const char *const CVS_OUTPUT_NODE_NAMES[NODE_COUNT] = {
    "RA",
    "RV",
    "P",
    "LA",
    "LV",
    "AO",
    "V",
};

void cvs_write_output_header(FILE *output_file)
{
    fprintf(output_file, "t");

    for (int i = 0; i < NODE_COUNT; ++i)
    {
        fprintf(output_file, ",V_%s", CVS_OUTPUT_NODE_NAMES[i]);
    }

    for (int i = 0; i < NODE_COUNT; ++i)
    {
        fprintf(output_file, ",P_%s", CVS_OUTPUT_NODE_NAMES[i]);
    }

    for (int i = 0; i < NODE_COUNT; ++i)
    {
        fprintf(output_file, ",Q_%s", CVS_OUTPUT_NODE_NAMES[i]);
    }

    fprintf(output_file, ",e_LV,e_RV\n");
}

void cvs_write_output_row(FILE *output_file, CVS_Simulation sim, const CVS_Parameters *params, const CVS_States *states)
{
    fprintf(output_file, "%.9f", sim.t);

    for (int i = 0; i < NODE_COUNT; ++i)
    {
        CVS_Node node = CVS_OUTPUT_NODE_ORDER[i];
        fprintf(output_file, ",%.9f", (double)states->V[node]);
    }

    for (int i = 0; i < NODE_COUNT; ++i)
    {
        CVS_Node node = CVS_OUTPUT_NODE_ORDER[i];
        fprintf(output_file, ",%.9f", (double)states->P[node]);
    }

    for (int i = 0; i < NODE_COUNT; ++i)
    {
        CVS_Node node = CVS_OUTPUT_NODE_ORDER[i];
        fprintf(output_file, ",%.9f", (double)states->Q[node]);
    }

    fprintf(output_file,
            ",%.9f,%.9f\n",
            (double)params->e[NODE_LV],
            (double)params->e[NODE_RV]);
}
