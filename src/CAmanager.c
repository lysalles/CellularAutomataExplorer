#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
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

#define SIZE(x) ((int) (sizeof(x) / sizeof(x[0])))
void runManyICA(void)
{
	int Ls[] = {/*50, 100,*/ 250/*, 500, 1000, 2000*/};
	float Qs[] = {
		.1, .2, .3, .4, .5, .6, .7, .8, .9,
		1, 1.2, 1.4, 1.6, 1.8, 
		2, 2.2, 2.4, 2.6, 2.8, 
		3, 3.2, 3.4, 3.6, 3.8,
		4, 4.2, 4.4, 4.6, 4.8,
		5, 5.2, 5.4, 5.6, 5.8,
		6, 6.2, 6.4, 6.6, 6.8,
		7, 7.2, 7.4, 7.6, 7.8,
		8, 8.2, 8.4, 8.6, 8.8,
		9, 9.2, 9.4, 9.6, 9.8, 10,
		11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
		22, 24, 26, 28, 30, 32, 34, 36, 38, 40,
		42, 44, 46, 48, 50, 52, 54, 56, 58, 60,
		65, 70, 75, 80, 85, 90, 95, 100, 110,
		120, 130, 140, 150, 160, 170, 180, 190,
		200, 220, 240, 260, 280, 300, 320, 340, 360, 380,
		400, 420, 440, 460, 480, 500, 520, 540, 560, 580,
		600, 620, 640, 660, 680, 700, 720, 740, 760, 780,
		800, 820, 840, 860, 880, 900, 920, 940, 960, 980,
		1000
		};
	int cycles = 20000;
	int repetitions = 5;

	for (int r = 0; r < repetitions; ++r)
	{
		for (int qi = 0; qi < SIZE(Qs); ++qi)
		{
			for (int Li = 0; Li < SIZE(Ls); ++Li)
			{
				ICA_new(Ls[Li], Qs[qi]);
				cycle = 0;
				
				if (data_stream != NULL)
					fclose(data_stream);
				sprintf(data_filename, "ICA-L%d-q%d_%3.3d-seed%u.dat", ICA_getL(), (int) ICA_getQ(), (int)(1000 * (ICA_getQ() - (int) ICA_getQ())), ICA_getSeed());
				data_stream = fopen(data_filename, "w");

				printf("\n\nStarting simulation for q=%g L=%d\n\n", Qs[qi], Ls[Li]);
				//fprintf(stdout, "%6s %9s %9s %9s\n", "Cycle", "AvgState", "AvgThres", "Clusters");
				fprintf(data_stream, "%6s \"%9s\" \"%9s\" %9s\n", "Cycle", "Average State", "Average Threshold", "Clusters");

				for (int i = cycle; i < cycles + cycle; ++i)
				{
					ICA_updateStats();
					//fprintf(stdout, "%6d %9.4f %9.4f %9d\n", i, ICA_getAvgState(), ICA_getAvgThres(), ICA_getNumberOfClusters());
					fprintf(data_stream, "%6d %9.4f %9.4f %9d\n", i, ICA_getAvgState(), ICA_getAvgThres(), ICA_getNumberOfClusters());
					ICA_run(1,0);
				}
				cycle += cycles;
				ICA_updateStats();
				fprintf(stdout, "%6d %9.4f %9.4f %9d\n", cycle, ICA_getAvgState(), ICA_getAvgThres(), ICA_getNumberOfClusters());
				fprintf(data_stream, "%6d %9.4f %9.4f %9d\n", cycle, ICA_getAvgState(), ICA_getAvgThres(), ICA_getNumberOfClusters());

				render();
			}
		}
	}
}

void startSimulation(void)
{
	int cycles;
	printf("Starting simulation for %s.\n", ICA_TITLE);
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
	fprintf(plot_stream, "plot \"%s\" using 1:2 with lines ls 1, \"%s\" using 1:3 with lines ls 1 lt 3\n\n", data_filename, data_filename);
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
