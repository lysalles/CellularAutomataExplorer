#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "CAmanager.h"
#include "CA/ICA.h"

typedef enum matrix_t {ICA} matrix_t;
static int cycle;
static char data_filename[200];
static FILE *data_stream = NULL;
static char plot_filename[200];
static FILE *plot_stream = NULL;

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
	
	if (data_stream != NULL)
		fclose(data_stream);
	sprintf(data_filename, "ICA-L%d-q%d_%3.3d-seed%u.dat", ICA_getL(), (int) ICA_getQ(), (int)(1000 * (ICA_getQ() - (int) ICA_getQ())), ICA_getSeed());
	data_stream = fopen(data_filename, "w");
}

void startSimulationVonNeumann(void)
{
	int cycles;
	printf("Starting simulation for %s with VonNeumann Neighborhood.\n", ICA_TITLE);
	printf("Enter number of cycles: ");
	scanf(" %d", &cycles);
	while(getchar() != '\n');

	fprintf(stdout, "%6s %9s %9s %9s\n", "Cycle", "AvgState", "AvgThres", "Clusters");
	fprintf(data_stream, "%6s \"%9s\" \"%9s\" %9s\n", "Cycle", "Average State", "Average Threshold", "Clusters");

	for (int i = cycle; i < cycles + cycle; ++i)
	{
		ICA_updateStats();
		fprintf(stdout, "%6d %9.4f %9.4f %9d\n", i, ICA_getAvgState(), ICA_getAvgThres(), ICA_getNumberOfClusters());
		fprintf(data_stream, "%6d %9.4f %9.4f %9d\n", i, ICA_getAvgState(), ICA_getAvgThres(), ICA_getNumberOfClusters());
		ICA_runVonNeumann(1,0);
	}
	cycle += cycles;
	ICA_updateStats();
	fprintf(stdout, "%6d %9.4f %9.4f %9d\n", cycle, ICA_getAvgState(), ICA_getAvgThres(), ICA_getNumberOfClusters());
	fprintf(data_stream, "%6d %9.4f %9.4f %9d\n", cycle, ICA_getAvgState(), ICA_getAvgThres(), ICA_getNumberOfClusters());
}

void startSimulationMoore(void)
{
	int cycles;
	printf("Starting simulation for %s with Moore Neighborhood.\n", ICA_TITLE);
	printf("Enter number of cycles: ");
	scanf(" %d", &cycles);
	while(getchar() != '\n');

	fprintf(stdout, "%6s %9s %9s %9s\n", "Cycle", "AvgState", "AvgThres", "Clusters");
	fprintf(data_stream, "%6s \"%9s\" \"%9s\" %9s\n", "Cycle", "Average State", "Average Threshold", "Clusters");

	for (int i = cycle; i < cycles + cycle; ++i)
	{
		ICA_updateStats();
		fprintf(stdout, "%6d %9.4f %9.4f %9d\n", i, ICA_getAvgState(), ICA_getAvgThres(), ICA_getNumberOfClusters());
		fprintf(data_stream, "%6d %9.4f %9.4f %9d\n", i, ICA_getAvgState(), ICA_getAvgThres(), ICA_getNumberOfClusters());
		ICA_runMoore(1,0);
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
	sprintf(plot_filename, "ICA-L%d-q%d_%3.3d-seed%u.plt", ICA_getL(), (int) ICA_getQ(), (int)(1000 * (ICA_getQ() - (int) ICA_getQ())), ICA_getSeed());
	plot_stream = fopen(plot_filename, "w");

	fprintf(plot_stream, "%s\n", "set key autotitle columnhead");
	fprintf(plot_stream, "%s\n", "set key outside bmargin left bottom height 2 width - 0 Left reverse horizontal");
	fprintf(plot_stream, "%s\n", "set autoscale y");
	fprintf(plot_stream, "%s\n", "set autoscale x");
	fprintf(plot_stream, "%s\n", "set term pngcairo enhanced font \"Times New Roman-Bold, 12\" size (16*38),(9*38)");
	fprintf(plot_stream, "%s\n", "set termoption dashed");
	fprintf(plot_stream, "%s\n", "set style line 1 lc rgb '#000000' lt 1 lw 1 pt 1 pi -1 ps 1.5");
	fprintf(plot_stream, "set title \"L=%d  q=%g  seed=%u\"\n\n", ICA_getL(), ICA_getQ(), ICA_getSeed());

	// Avg state & Avg threshold vs cycle
	fprintf(plot_stream, "set ylabel \"Average value\"\n");
	fprintf(plot_stream, "set xlabel\"Cycle\"\n");
	fprintf(plot_stream, "set output \"ICA-L%d-q%d_%d-seed%u-AvgStateAvgThresVsCycle.png\"\n", ICA_getL(), (int) ICA_getQ(), (int)(1000 * (ICA_getQ() - (int) ICA_getQ())), ICA_getSeed());
	fprintf(plot_stream, "plot [0:500] \"%s\" using 1:2 with lines ls 1, \"%s\" using 1:3 with lines ls 1 lt 3\n\n", data_filename, data_filename);
	// Clusters vs AvgState
	fprintf(plot_stream, "set ylabel \"Num. of Clusters\"\n");
	fprintf(plot_stream, "set xlabel \"Average State\"\n");
	fprintf(plot_stream, "f(x) = a*x+b\n");
	fprintf(plot_stream, "fit f(x) \"%s\" using 2:4 via a, b\n", data_filename);
	fprintf(plot_stream, "set output \"ICA-L%d-q%d_%d-seed%u-ClusterVsAvgState.png\"\n", ICA_getL(), (int) ICA_getQ(), (int)(1000 * (ICA_getQ() - (int) ICA_getQ())), ICA_getSeed());
	fprintf(plot_stream, "plot \"%s\" using 2:4 lc rgb '#000000', f(x) t sprintf(\"f(x) = %%.0f x + %%.0f\", a, b) lt 1 lc rgb '#888888' lw 3\n\n", data_filename);
	// Clusters vs AvgThres
	fprintf(plot_stream, "set ylabel \"Num. of Clusters\"\n");
	fprintf(plot_stream, "set xlabel \"Average Threshold\"\n");
	fprintf(plot_stream, "unset key\n");
	fprintf(plot_stream, "set output \"ICA-L%d-q%d_%d-seed%u-ClusterVsAvgThres.png\"\n", ICA_getL(), (int) ICA_getQ(), (int)(1000 * (ICA_getQ() - (int) ICA_getQ())), ICA_getSeed());
	fprintf(plot_stream, "plot \"%s\" using 3:4 lc rgb '#000000'\n", data_filename);

	if (plot_stream != NULL)
		fclose(plot_stream);
}

void drawState(void)
{
	ICA_printState();
}

void drawThres(void)
{
	ICA_printThres();
}
