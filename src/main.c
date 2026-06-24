#include <stdio.h>
#include "cvs_elastances.h"

int main(void)
{
    CVS_Model cvs = {0};

    float dt = 1e-3; // 0.001 10^-3
    for (float i = 0; i < 40; i += dt)
    {
        cvs.time = i;

        cvs_calculate_e_RV(&cvs);

        float e_RV = cvs.p.e.RV; 

        printf("e_RV = %f\n", e_RV);
    }

    return 0;
}