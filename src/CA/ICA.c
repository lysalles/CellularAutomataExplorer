#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "ICA.h"

typedef struct
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
static int countClusters(void);

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

	free(ICA.matrix);
	newMatrix = (cell *) calloc((L + 2) * (L + 2), sizeof(cell));

	if (newMatrix == NULL)
	{
		printf("Failed to create new %s with L=%d and q=%g\n", ICA_TITLE, L, q);
		while(getchar() != '\n');
	}
	else
	{
		newSeed = (unsigned int) time(NULL);
		srand(newSeed);

		for (int row = 1; row < L + 1; ++row)
			for (int register col = 1; col < L + 1; ++col)
			{ 
				(newMatrix + row * (L + 2) + col)->state = rand() % 2 * 2 - 1;
				(newMatrix + row * (L + 2) + col)->threshold = (rand() % 10000 / 10000.0 * 2 - 1.0) * q;
			}
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
			deltaQ = rand() % 10000 / 10000.0f * ICA.q;

			cell *c = (ICA.matrix + y * ICA.L + x);

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

void ICA_updateStats(void)
{
	double totalState = 0;
	double totalThreshold = 0.0;

	cell *lastElement = ICA.matrix + (ICA.L + 2) * (ICA.L + 1);

	for (register cell *i = ICA.matrix; i < lastElement; ++i)
	{
		totalState += i->state;
		totalThreshold += i->threshold;
	}
		
	ICA.avgState = totalState / (ICA.L * ICA.L);
	ICA.avgThres = totalThreshold / (ICA.L * ICA.L);
	ICA.numberOfClusters = countClusters();
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

static int countClusters(void) // 2D
{
	// IMPLEMENTATION INDEPPENDENT
	int numberOfClusters = 0;
	int newCluster;
	int row, col;
	int maxRows, maxCols;
	
	// IMPLEMENTATION DEPENDENT
	cell *N, *S, *E, *W, *C;
	int8_t * const cellCounted = calloc((ICA.L + 2) * (ICA.L + 2), sizeof(int8_t));
	maxRows = ICA.L;
	maxCols = ICA.L;
	// queue
	cell ** const queue = calloc(4 * ICA.L, sizeof(cell *));
	int first = 0, last = 0, elements = 0;

	// ALGORITHM
	for (row = 1; row <= maxRows; ++row)
		for (col = 1; col <= maxCols; ++col)
		{
			C = ICA.matrix + (ICA.L + 2) * row + col;

			if (C->state != 1)
				continue;

			if (*(cellCounted + (C - ICA.matrix)) == 1)
				continue;

			newCluster = 0;


			*(cellCounted + (C - ICA.matrix)) = 1;

			N = C - (ICA.L + 2);
			S = C + (ICA.L + 2);
			E = C + 1;
			W = C - 1;

			if (N->state == 1)
			{
				if (*(cellCounted + (N - ICA.matrix)) == 0)
				{
					newCluster = 1;
					*(queue + last) = N; //enqueue
					last = last + 1 < 4 * ICA.L ? last + 1 : 0;
					++elements;
				}
			} 
			if (S->state == 1)
			{
				if (*(cellCounted + (S - ICA.matrix)) == 0)
				{
					newCluster = 1;
					*(queue + last) = S; //enqueue
					last = last + 1 < 4 * ICA.L ? last + 1 : 0;
					++elements;
				}
			} 
			if (E->state == 1)
			{
				if (*(cellCounted + (E - ICA.matrix)) == 0)
				{
					newCluster = 1;
					*(queue + last) = E; //enqueue
					last = last + 1 < 4 * ICA.L ? last + 1 : 0;
					++elements;
				}
			} 
			if (W->state == 1)
			{
				if (*(cellCounted + (W - ICA.matrix)) == 0)
				{
					newCluster = 1;
					*(queue + last) = W; //enqueue
					last = last + 1 < 4 * ICA.L ? last + 1 : 0;
					++elements;
				}
			} 
			if (newCluster = 1)
				++numberOfClusters;

			while (elements > 0)
			{
				C = *(queue + first); // dequeue
				*(cellCounted + (C - ICA.matrix)) = 1;
				first = first + 1 < 4 * ICA.L ? first + 1 : 0;
				--elements;

				N = C - (ICA.L + 2);
				S = C + (ICA.L + 2);
				E = C + 1;
				W = C - 1;

				if (N->state == 1)
				{
					if (*(cellCounted + (N - ICA.matrix)) == 0)
					{
						*(queue + last) = N; //enqueue
						last = last + 1 < 4 * ICA.L ? last + 1 : 0;
						++elements;
					}
				} 
				if (S->state == 1)
				{
					if (*(cellCounted + (S - ICA.matrix)) == 0)
					{
						*(queue + last) = S; //enqueue
						last = last + 1 < 4 * ICA.L ? last + 1 : 0;
						++elements;
					}
				} 
				if (E->state == 1)
				{
					if (*(cellCounted + (E - ICA.matrix)) == 0)
					{
						*(queue + last) = E; //enqueue
						last = last + 1 < 4 * ICA.L ? last + 1 : 0;
						++elements;
					}
				} 
				if (W->state == 1)
				{
					if (*(cellCounted + (W - ICA.matrix)) == 0)
					{
						*(queue + last) = W; //enqueue
						last = last + 1 < 4 * ICA.L ? last + 1 : 0;
						++elements;
					}
				} 
			}
		}
		
	free(queue);
	free(cellCounted);

	return numberOfClusters;
}
