#include <stdio.h>
#include <stdint.h>
#include "CAmanager.h"
#include "CA/ICA.h"

typedef enum matrix_t {ICA} matrix_t;

typedef struct property
{
	const char *name;
	double (*getValue)(void);
	void (*setValue)(double);
	struct property *next;
} property;

struct matrix
{
	matrix_t type;
	int width;
	int height;

	property *properties;

	void (*new)(void);
	void (*delete)(void);
	void (*run)(uint64_t steps);
};


void newMatrix(void)
{
	int L;
	float q;
	printf("Creating new %s.\n", ICA_TITLE);
	printf("Enter the value for L: L=");
	scanf(" %d", &L);
	while(getchar() != '\n');
	printf("Enter the value for q: q=");
	scanf(" %f", &q);
	while(getchar() != '\n');
	ICA_new(L, q);
}

void startSimulation(void)
{
	int cycles;
	printf("Starting simulation for %s.\n", ICA_TITLE);
	printf("Enter number of cycles: ");
	scanf(" %d", &cycles);
	while(getchar() != '\n');

	printf("%6s %9s %9s %9s\n", "Cycle", "AvgState", "AvgThres", "Clusters");

	for (int i = 0; i <= cycles; ++i)
	{
		ICA_run(1,0);
		ICA_updateStats();
		printf("%6d %9.4f %9.4f %9d\n", i, ICA_getAvgState(), ICA_getAvgThres(), ICA_getNumberOfClusters());
	}
}
