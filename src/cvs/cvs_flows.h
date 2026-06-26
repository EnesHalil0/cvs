#ifndef CVS_FLOWS_H
#define CVS_FLOWS_H

#include "cvs_model.h"

void cvs_initialize_flow_defaults(CVS_Parameters *params);
void cvs_calculate_flows(CVS_Parameters *params, CVS_States *states);

#endif // CVS_FLOWS_H
