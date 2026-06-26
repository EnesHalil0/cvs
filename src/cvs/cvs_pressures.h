#ifndef CVS_PRESSURES_H
#define CVS_PRESSURES_H

#include "cvs_model.h"

void cvs_initialize_pressure_defaults(CVS_Parameters *params);
void cvs_calculate_pressures(CVS_Parameters *params, CVS_States *states);

#endif // CVS_PRESSURES_H
