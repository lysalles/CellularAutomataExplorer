#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "CAmanager.h"
#include "CA/ICA.h"

typedef enum matrix_t {ICA} matrix_t;
static int cycle;
static char data_filename[200];
static FILE *data_stream;
static char plot_filename[200];
static FILE *plot_stream;

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
	sprintf(plot_filename, "ICA-L%d-q%d_%d_seed%u.plt", ICA_getL(), (int) ICA_getQ(), (int)(ICA_getQ() - (int) ICA_getQ()), ICA_getSeed());
	sprintf(data_filename, "ICA-L%d-q%d_%d_seed%u.dat", ICA_getL(), (int) ICA_getQ(), (int)(ICA_getQ() - (int) ICA_getQ()), ICA_getSeed());
	cycle = 0;
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
	data_stream = fopen(data_filename, "w");
	int cycles;
	printf("Starting simulation for %s.\n", ICA_TITLE);
	printf("Enter number of cycles: ");
	scanf(" %d", &cycles);
	while(getchar() != '\n');

	fprintf(stdout, "%6s %9s %9s %9s\n", "Cycle", "AvgState", "AvgThres", "Clusters");
	fprintf(data_stream, "%6s %9s %9s %9s\n", "Cycle", "AvgState", "AvgThres", "Clusters");

	for (int i = cycle; i < cycles + cycle; ++i)
	{
		ICA_updateStats();
		fprintf(stdout, "%6d %9.4f %9.4f %9d\n", i, ICA_getAvgState(), ICA_getAvgThres(), ICA_getNumberOfClusters());
		fprintf(data_stream, "%6d %9.4f %9.4f %9d\n", i, ICA_getAvgState(), ICA_getAvgThres(), ICA_getNumberOfClusters());
		ICA_run(1,0);
	}
	cycle += cycles;
	ICA_updateStats();
	fprintf(stdout, "%6d %9.4f %9.4f %9d\n", cycle, ICA_getAvgState(), ICA_getAvgThres(), ICA_getNumberOfClusters());
	fprintf(data_stream, "%6d %9.4f %9.4f %9d\n", cycle, ICA_getAvgState(), ICA_getAvgThres(), ICA_getNumberOfClusters());
}

void analyse(void)
{
	ICA_updateStats();
	printf("Type: %s\n", ICA_TITLE);
	printf("L=%d  q=%g\n", ICA_getL(), ICA_getQ());
	printf("Seed = %u\n", ICA_getSeed());
	printf("+1 Cells %% = %g\n", 100.0*(ICA_getAvgState() + 1)/2.0);
	printf("-1 Cells %% = %g\n", 100.0*(1 - (ICA_getAvgState() + 1)/2.0));
	printf("Average state = %g\n", ICA_getAvgState());
	printf("Average Threshold = %g\n", ICA_getAvgThres());
	printf("Number of positive clusters = %d\n", ICA_getNumberOfClusters());
}

void render(void)
{
	plot_stream = fopen(plot_filename, "w");

}
