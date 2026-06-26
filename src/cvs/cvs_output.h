#ifndef CVS_OUTPUT_H
#define CVS_OUTPUT_H

#include <stdio.h>

#include "cvs_model.h"

void cvs_write_output_header(FILE *output_file);
void cvs_write_output_row(FILE *output_file, CVS_Simulation sim, const CVS_Parameters *params, const CVS_States *states);

#endif // CVS_OUTPUT_H
