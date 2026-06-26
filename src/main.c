#include <stdio.h>
#include "cvs_elastances.h"
#include "cvs_flows.h"
#include "cvs_output.h"
#include "cvs_pressures.h"
#include "cvs_volumes.h"

int main(void)
{
    CVS_Simulation cvs_simulation = {
        .t = 0.0,
        .dt = 1e-4,
        .duration = 40.0,
    };

    CVS_Parameters cvs_parameters = {
        .heart_rate = 60.0f,
    };

    CVS_States cvs_states = {0};
    FILE *output_file = fopen("output.csv", "w");

    if (output_file == NULL)
    {
        fprintf(stderr, "Failed to open output.csv\n");
        return 1;
    }

    cvs_initialize_volume_defaults(&cvs_states);
    cvs_initialize_pressure_defaults(&cvs_parameters);
    cvs_initialize_flow_defaults(&cvs_parameters);
    cvs_write_output_header(output_file);

    int step_count = (int)((cvs_simulation.duration / cvs_simulation.dt) + 0.5f);

    for (int step = 0; step < step_count; ++step)
    {
        cvs_simulation.t = (float)(step + 1) * cvs_simulation.dt;

        cvs_calculate_elastances(&cvs_parameters, cvs_states, cvs_simulation);
        cvs_calculate_pressures(&cvs_parameters, &cvs_states);
        cvs_calculate_flows(&cvs_parameters, &cvs_states);
        cvs_calculate_volumes(&cvs_states, cvs_simulation);
        cvs_write_output_row(output_file, cvs_simulation, &cvs_parameters, &cvs_states);
    }

    fclose(output_file);
    printf("Simulation complete: output.csv\n");

    return 0;
}
