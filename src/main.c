#include <stdio.h>
#include "cvs_core.h"
#include "cvs_output.h"

int main(void)
{
    CVS_ModelConfig cvs_config = {
        .heart_rate = 60.0f,
        .dt = 1e-4,
        .duration = 40.0,
        .heart_failure = true,
        .exercise = false,
        .solver_type = CVS_SOLVER_EULER,
    };

    CVS_Model cvs_model;
    FILE *output_file = fopen("output.csv", "w");

    if (output_file == NULL)
    {
        fprintf(stderr, "Failed to open output.csv\n");
        return 1;
    }

    cvs_model_init(&cvs_model, &cvs_config);
    cvs_write_output_header(output_file);

    int step_count = (int)((cvs_config.duration / cvs_config.dt) + 0.5f);

    for (int step = 0; step < step_count; ++step)
    {
        cvs_model_step(&cvs_model);
        cvs_write_output_row(output_file,
                             cvs_model_get_simulation(&cvs_model),
                             cvs_model_get_params(&cvs_model),
                             cvs_model_get_state(&cvs_model));
    }

    fclose(output_file);
    printf("Simulation complete: output.csv\n");

    return 0;
}
