#ifndef CVS_VOLUMES_H
#define CVS_VOLUMES_H

#include "cvs_model.h"

void cvs_initialize_volume_defaults(CVS_States *states);
void cvs_calculate_volumes(CVS_States *states, CVS_Simulation sim);

#endif // CVS_VOLUMES_H
