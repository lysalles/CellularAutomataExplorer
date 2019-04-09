#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "ICA.h"
#include <stdbool.h>

// CLUSTER.H BEGIN

typedef struct matrix_t {void *m; int rows; int cols;} matrix_t;
typedef struct neighbor_t {void *N, *S, *E, *W;} neighbor_t;

int countClusters(const matrix_t *m);

// CLUSTER.H END

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
void ICA_printMatrix(void);

// VALUE RETURNING FUNCTIONS

float ICA_getL(void){return ICA.L;}
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

void ICA_printMatrix(void)
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
	ICA.numberOfClusters = countClusters(&(matrix_t) {(void *)ICA.matrix, ICA.L, ICA.L});

	ICA_printMatrix();
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

int countClusters(const matrix_t *m) // 2D, closed borders
{
	// IMPLEMENTATION INDEPENDENT
	int maxRows = m->rows, maxCols = m->cols;

	int (*getCellState)(void *c);
	void *(*getCell)(int row, int col, const matrix_t *m);
	neighbor_t *(*getNeighbor)(void *c, const matrix_t *m);
	
	bool * const cellCounted = calloc((maxRows + 2) * (maxCols + 2), sizeof(bool));
	int numberOfClusters = 0;
	bool newCluster;
	int row, col;
	neighbor_t *neighbor;

	// queue
	int queueSize = 2 * maxCols + 2 * maxRows;
	void **queue = calloc(queueSize, sizeof(void *));
	int first = 0, last = 0, elements = 0;
	#define ENQUEUE(x) *(queue + last) = x; last = last + 1 < queueSize ? last + 1 : 0; ++elements
	#define DEQUEUE *(queue + first); first = first + 1 < queueSize ? first + 1 : 0; --elements

	// IMPLEMENTATION DEPENDENT
	cell *matrix = m->m;
	cell *N, *S, *E, *W, *C;
	getCellState = ICA_getCellState;
	getCell = ICA_getCellByPos;
	getNeighbor = ICA_getNeighbor;

	// ALGORITHM
	for (row = 1; row <= maxRows; ++row)
		for (col = 1; col <= maxCols; ++col)
		{
			C = getCell(row, col, m);

			if (getCellState(C) != 1 || *(cellCounted + (C - matrix)) == true)
				continue;

			newCluster = false;

			ENQUEUE(C);

			while (elements > 0)
			{
				C = DEQUEUE;

				*(cellCounted + (C - matrix)) = true;

				neighbor = getNeighbor(C, m);
				N = neighbor->N;
				S = neighbor->S;
				E = neighbor->E;
				W = neighbor->W;
				
				if (getCellState(N) == 1 && *(cellCounted + (N - matrix)) == false)
				{
					newCluster = true;
					ENQUEUE(N);
				} 
				if (getCellState(S) == 1 && *(cellCounted + (S - matrix)) == false)
				{
					newCluster = true;
					ENQUEUE(S);
				} 
				if (getCellState(E) == 1 && *(cellCounted + (E - matrix)) == false)
				{
					newCluster = true;
					ENQUEUE(E);
				} 
				if (getCellState(W) == 1 && *(cellCounted + (W - matrix)) == false)
				{
					newCluster = true;
					ENQUEUE(W);
				} 
			}

			if (newCluster) ++numberOfClusters;
		}
		
	free(queue);
	free(cellCounted);

	return numberOfClusters;
}

