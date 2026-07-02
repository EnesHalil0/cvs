#ifndef CVS_MODEL_H
#define CVS_MODEL_H

#include <stdbool.h>

/*
 * CVS_Node: parametre arraylerinde daha rahat kullanabilmek icin aslinda
 * bir indeksi her zaman bir CVS Node'una atamis oluyoruz.
 */
typedef enum
{
	NODE_RA, // Right Atrium
	NODE_RV, // Right Ventricle
	NODE_P,	 // Pulmonary circulation
	NODE_LA, // Left Atrium
	NODE_LV, // Left Ventricle
	NODE_AO, // Aorta
	NODE_V,	 // Veins

	NODE_COUNT // Number of node
} CVS_Node;

/*
 * CVS_Parameters: CVS'in karakteristigini belirleyen parametreler
 * farkli kosullarda genelde bunlar degisiyor.
 */
typedef struct
{
	float heart_rate;

	float e[NODE_COUNT];
	float g[NODE_COUNT];
	float V0[NODE_COUNT];

	bool heart_failure;
	bool exercise;
} CVS_Parameters;

/*
 * CVS_States: Genelde gozlemledigimiz ve simulasyonu dondurme amaclarimiz
 */
typedef struct
{
	float P[NODE_COUNT];
	float Q[NODE_COUNT];
	float V[NODE_COUNT];
} CVS_States;

typedef struct
{
	float t;
	float dt;
	float duration;
} CVS_Simulation;

#endif // CVS_MODEL_H
