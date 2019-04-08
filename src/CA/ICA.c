#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "ICA.h"
#include <stdbool.h>

#define DELTAQ_PRECISION 100000

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
	float new_q;

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

		for (int row = 1; row < L + 1; ++row)
			for (int register col = 1; col < L + 1; ++col)
			{ 
				(newMatrix + row * (L + 2) + col)->state = rand() % 2 * 2 - 1;
				new_q = (rand() % DELTAQ_PRECISION * 2.0 / DELTAQ_PRECISION - 1.0) * q;
				(newMatrix + row * (L + 2) + col)->threshold = new_q;
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

static int countClusters(void) // 2D, closed borders
{
	// IMPLEMENTATION INDEPENDENT
	int numberOfClusters = 0;
	bool newCluster;
	int row, col;
	int maxRows, maxCols;
	int L;
	
	// queue
	int queueSize;
	int first = 0, last = 0, elements = 0;
	void **queue;
	#define ENQUEUE(x) *(queue + last) = x; last = last + 1 < queueSize ? last + 1 : 0; ++elements
	#define DEQUEUE *(queue + first); first = first + 1 < queueSize ? first + 1 : 0; --elements

	// IMPLEMENTATION DEPENDENT
	cell *matrix = ICA.matrix;
	cell *N, *S, *E, *W, *C;
	L = ICA.L;
	maxRows = L;
	maxCols = L;
	bool * const cellCounted = calloc((maxRows + 2) * (maxCols + 2), sizeof(bool));
	queueSize = 4 * L;
	queue = calloc(queueSize, sizeof(cell *));

	// ALGORITHM
	for (row = 1; row <= maxRows; ++row)
		for (col = 1; col <= maxCols; ++col)
		{
			C = matrix + (L + 2) * row + col;

			if (C->state != 1 || *(cellCounted + (C - matrix)) == true)
				continue;

			newCluster = false;

			*(cellCounted + (C - matrix)) = true;

			N = C - (L + 2);
			S = C + (L + 2);
			E = C + 1;
			W = C - 1;

			if (N->state == 1 && *(cellCounted + (N - matrix)) == false)
			{
				newCluster = true;
				ENQUEUE(N);
			} 
			if (S->state == 1 && *(cellCounted + (S - matrix)) == false)
			{
				newCluster = true;
				ENQUEUE(S);
			} 
			if (E->state == 1 && *(cellCounted + (E - matrix)) == false)
			{
				newCluster = true;
				ENQUEUE(E);
			} 
			if (W->state == 1 && *(cellCounted + (W - matrix)) == false)
			{
				newCluster = true;
				ENQUEUE(W);
			} 
			if (newCluster = 1)
				++numberOfClusters;

			while (elements > 0)
			{
				C = DEQUEUE;

				*(cellCounted + (C - matrix)) = 1;

				N = C - (L + 2);
				S = C + (L + 2);
				E = C + 1;
				W = C - 1;

				if (N->state == 1 && *(cellCounted + (N - matrix)) == false)
				{
					newCluster = true;
					ENQUEUE(N);
				} 
				if (S->state == 1 && *(cellCounted + (S - matrix)) == false)
				{
					newCluster = true;
					ENQUEUE(S);
				} 
				if (E->state == 1 && *(cellCounted + (E - matrix)) == false)
				{
					newCluster = true;
					ENQUEUE(E);
				} 
				if (W->state == 1 && *(cellCounted + (W - matrix)) == false)
				{
					newCluster = true;
					ENQUEUE(W);
				} 
			}
		}
		
	free(queue);
	free(cellCounted);

	return numberOfClusters;
}
