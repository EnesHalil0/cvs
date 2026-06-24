#include <stdbool.h>

extern float time;

typedef struct
{
    bool heart_failure;
    bool exercise;
} CVS_Conditions;

typedef struct
{
    float RV;
} CVS_Elastances;

typedef struct
{
    CVS_Elastances e;
} CVS_Params;

typedef struct
{
    float RV;
} CVS_Volumes;

typedef struct
{
    CVS_Volumes V;
} CVS_States;

typedef struct
{
    float heart_rate;
    CVS_Conditions c;
    CVS_Params p;
    CVS_States s;
} CVS_Model;