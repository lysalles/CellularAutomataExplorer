#include <stdlib.h>
#include <stdint.h>
#include <time.h>

typedef struct
{
	int state;
	float threshold;
} cell;

struct 
{
	cell *matrix;
	int L;
	float q;
	float avgState;
	float avgThres;
} ICA = {0};

void ICA_new(int L)
{
	srand((unsigned int) time(NULL));
	free(ICA.matrix);
	ICA.matrix = calloc((L + 2) * (L + 2), sizeof(cell));
}

void ICA_delete(void)
{
	free(ICA.matrix);
}

int ICA_neighborSum(int x, int y)
{
	int sum;

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
	int positiveCells = 0;
	double totalThreshold = 0.0;

	cell *lastElement = ICA.matrix + (ICA.L + 2) * (ICA.L + 1);

	for (register cell *i = ICA.matrix; i < lastElement; ++i)
	{
		positiveCells += i->state == 1;
		totalThreshold += i->threshold;
	}
		
	ICA.avgState = (2.0 * positiveCells) / (ICA.L * ICA.L) - 1.0;
	ICA.avgThres = totalThreshold / (ICA.L * ICA.L);
}
