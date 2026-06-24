#include <stdbool.h>

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
    float RV;
} CVS_Pressures;

typedef struct
{
    float RV;
} CVS_Flows;

typedef struct
{
    CVS_Volumes V;
    CVS_Pressures P;
    CVS_Flows Q;
} CVS_States;

typedef struct
{
    float time;
    float heart_rate;
    CVS_Conditions c;
    CVS_Params p;
    CVS_States s;
} CVS_Model;