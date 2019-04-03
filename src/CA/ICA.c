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
} ICA = {NULL};

float ICA_getL(void)
{
	return ICA.L;
}

float ICA_getQ(void)
{
	return ICA.q;
}

float ICA_getAvgState(void)
{
	return ICA.avgState;
}

float ICA_getAvgThres(void)
{
	return ICA.avgThres;
}

unsigned int ICA_getSeed(void)
{
	return ICA.seed;
}

void ICA_new(int L, float q)
{

	cell *newMatrix;
	unsigned int newSeed;

	free(ICA.matrix);
	newMatrix = (cell *) calloc((L + 2) * (L + 2), sizeof(cell));

	if (newMatrix == NULL)
	{
		printf("Failed to create new %s with L=%d and q=%g\n", ICA_TITLE, L, q);
		while(getchar() != '\n');
	}

	newSeed = (unsigned int) time(NULL);
	srand(newSeed);

	for (int row = 1; row < L + 1; ++row)
		for (int register col = 1; col < L + 1; ++col)
		{ 
			(newMatrix + row * (L + 2) + col)->state = rand() % 2 * 2 - 1;
			(newMatrix + row * (L + 2) + col)->threshold = (rand() % 10000 / 10000.0 * 2 - 1.0) * q;
		}

	ICA = (struct ICA) {newMatrix, L, q, 0, 0, newSeed};
}

void ICA_delete(void)
{
	free(ICA.matrix);
}

int ICA_neighborSum(int x, int y)
{
	int sum = 0;

	sum += (ICA.matrix + (y - 1) * ICA.L + (x))->state;
	sum += (ICA.matrix + (y + 1) * ICA.L + (x))->state;
	sum += (ICA.matrix + (y) * ICA.L + (x - 1))->state;
	sum += (ICA.matrix + (y) * ICA.L + (x + 1))->state;

	return sum;
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
		steps = cycles > 0 ? (--cycles, ICA.L * ICA.L) : 0;
	}
}

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
}
