#include <stdio.h>
#include "cvs_elastances.h"

int main(void)
{
    CVS_Simulation cvs_simulation = {
        .t = 0.0,
        .dt = 1e-3,
        .duration = 40.0,
    };

    CVS_Parameters cvs_parameters = {
        .heart_rate = 60.0f,
    };

    CVS_States cvs_states = {0};

    while (cvs_simulation.t < cvs_simulation.duration)
    {

        cvs_calculate_e_RV(cvs_parameters, cvs_states, cvs_simulation);

        printf("e_RV = %f\n", cvs_parameters.e[NODE_RV]);

        cvs_simulation.t += cvs_simulation.dt;
    }

    return 0;
}