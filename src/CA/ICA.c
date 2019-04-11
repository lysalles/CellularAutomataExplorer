#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include "ICA.h"
#include "tools/countCluster.h"

#define DELTAQ_PRECISION 100000

typedef struct cell
{
	int state;
	float threshold;
} cell;

struct ICA
{
	cell *matrix;
	int L;
	float q;
	float avgState;
	float avgThres;
	unsigned int seed;
	int numberOfClusters;
} ICA = {NULL};

// STATIC FUNCTIONS DECLARATIONS

static int ICA_neighborSum(int x, int y);
static void ICA_printMatrix(void);
static int ICA_getCellState(void *c);
static void *ICA_getCellByPos(int row, int col, const matrix_t *m);
static neighbor_t *ICA_getNeighbor(void *c, const matrix_t *m);
static uint64_t ICA_getCellPos(void *c, const matrix_t *m);

// VALUE RETURNING FUNCTIONS !! not prototypes !!

int ICA_getL(void){return ICA.L;}
float ICA_getQ(void){return ICA.q;}
float ICA_getAvgState(void){return ICA.avgState;}
float ICA_getAvgThres(void){return ICA.avgThres;}
unsigned int ICA_getSeed(void){return ICA.seed;}
int ICA_getNumberOfClusters(void){return ICA.numberOfClusters;}

// MAIN FUNCTIONS

void ICA_new(int L, float q)
{
	cell *newMatrix;
	unsigned int newSeed = 0;
	float new_q;
	cell *c;

	free(ICA.matrix);
	newMatrix = (cell *) calloc((L + 2) * (L + 2), sizeof(cell));

	printf("Creating new %s matrix...\n", ICA_TITLE);

	if (newMatrix == NULL)
	{
		printf("Failed to create new %s with L=%d and q=%g\n", ICA_TITLE, L, q);
		while(getchar() != '\n');
	}
	else
	{
		newSeed = (unsigned int) time(NULL);
		srand(newSeed);

		for (int row = 1; row <= L; ++row)
			for (register int col = 1; col <= L; ++col)
			{ 
				c = (newMatrix + row * (L + 2) + col);
				c->state = rand() % 2 * 2 - 1;
				new_q = (rand() % DELTAQ_PRECISION * 2.0 / DELTAQ_PRECISION - 1.0) * q;
				c->threshold = new_q;
			}

		puts("Successfully created.\n");
	}
	
	ICA.matrix = newMatrix;
	ICA.L = L;
	ICA.q = q;
	ICA.avgState = 0;
	ICA.avgThres = 0;
	ICA.numberOfClusters = 0;
	ICA.seed = newSeed;
}

void ICA_delete(void)
{
	if (ICA.matrix != NULL)
		puts("ICA matrix deleted.");
	free(ICA.matrix);
	ICA.matrix = NULL;
}

void ICA_run(int32_t cycles, int32_t steps)
{
	int x, y;
	float deltaQ;

	while (cycles > 0  || steps > 0)
	{
		for (;steps > 0; --steps)
		{
			x = rand() % ICA.L + 1;
			y = rand() % ICA.L + 1;

			deltaQ = rand() % DELTAQ_PRECISION * ICA.q / DELTAQ_PRECISION;

			cell *c = (ICA.matrix + y * (ICA.L + 2) + x);

			if (ICA_neighborSum(x, y) > c->threshold)
			{
				c->state = 1;
				c->threshold += deltaQ;
			}
			else
			{
				c->state = -1;
				c->threshold -= deltaQ;
			}
		}
		if (cycles > 0 )
		{
			--cycles;
			steps = ICA.L * ICA.L;
		}
	}
}

// INFORMATION RETRIEVAL FUNCTIONS

static void ICA_printMatrix(void)
{
	putchar('\n');
	for (int row = 0; row < ICA.L + 2; ++row)
	{	
		for (int col = 0; col < ICA.L + 2; ++col)
		{
			int state = (ICA.matrix + row * (ICA.L + 2) + col)->state;
			putchar(state == 1 ? '+' : ' ');
			putchar(' ');
		}
		
		putchar('\n');
	}		
}

void ICA_updateStats(void)
{
	double totalState = 0;
	double totalThreshold = 0.0;

	cell *lastElement = ICA.matrix + (ICA.L + 2) * (ICA.L + 1);

	for (cell *i = ICA.matrix; i < lastElement; ++i)
	{
		totalState += i->state;
		totalThreshold += i->threshold;
	}
		
	ICA.avgState = totalState / (ICA.L * ICA.L);
	ICA.avgThres = totalThreshold / (ICA.L * ICA.L);
	matrix_t m = (matrix_t) 
	{
		(void *)ICA.matrix, 
		ICA.L, 
		ICA.L, 
		ICA_getCellState, 
		ICA_getCellByPos, 
		ICA_getNeighbor, 
		ICA_getCellPos
	};
	ICA.numberOfClusters = countClusters(&m);
}

static int ICA_neighborSum(int x, int y)
{
	int sum = 0;

	sum += (ICA.matrix + (y - 1) * ICA.L + (x))->state;
	sum += (ICA.matrix + (y + 1) * ICA.L + (x))->state;
	sum += (ICA.matrix + (y) * ICA.L + (x - 1))->state;
	sum += (ICA.matrix + (y) * ICA.L + (x + 1))->state;

	return sum;
}

// TOOLS FOR COUNTING CLUSTER

static int ICA_getCellState(void *c)
{
	return ((cell *) c)->state;
}


static void *ICA_getCellByPos(int row, int col, const matrix_t *m)
{
	cell *M = m->m;
	return (void *) (M + row * (m->rows + 2) + col);
}

static neighbor_t *ICA_getNeighbor(void *c, const matrix_t *m)
{
	static neighbor_t neighbor;
	cell *C = c;
	neighbor = (neighbor_t)
	{
		C - (m->cols + 2),
		C + (m->cols + 2),
		C + 1,
		C - 1
	};
	return &neighbor;
}

static uint64_t ICA_getCellPos(void *c, const matrix_t *m)
{
	return ((cell *) c) - ((cell *) m->m);
}
