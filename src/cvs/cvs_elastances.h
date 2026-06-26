#ifndef CVS_ELASTANCES_H
#define CVS_ELASTANCES_H

#include "cvs_model.h"

void cvs_calculate_e_RV(CVS_Parameters *params, CVS_States states, CVS_Simulation sim);
void cvs_calculate_e_LV(CVS_Parameters *params, CVS_States states, CVS_Simulation sim);
void cvs_calculate_elastances(CVS_Parameters *params, CVS_States states, CVS_Simulation sim);

#endif // CVS_ELASTANCES_H
