#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ICA.h"
#include "../macroTools.h"

#define MAX_LINES 100
#define MAX_COLUMNS 100
#define Q_PRECISION 10000

struct ICA_cell 
{
	int8_t state;
	float threshold;
};

struct
{
	struct ICA_cell *matrix;
	long lines;
	long columns;
} ICA = { NULL, 0, 0 };

/*
ICA_neighborSum:
Returns the sum of the cells above, below, and besides the cell at the given coordinate.
If no cell is present on any of those positions, 0 is summed.
*/
int8_t ICA_neighborSum(long line, long column);

/*
ICA_getCell:
Returns the address of the cell at the given coordinates.
*/
struct ICA_cell *ICA_getCell(long line, long column);

/*
ICA_set_state:
Sets the state of the cell at the given coordinates with the given state.
*/
void ICA_set_state(long line, long column, int8_t state);

/*
ICA_set_threshold:
Sets the threshold of the cell at the given coordinates with the given state.
*/
void ICA_set_threshold(long line, long column, int8_t state);

void ICA_new(long lines, long columns)
{
	if (lines > MAX_LINES)
	{
		puts("Failed to create new " ICA_TITLE ": lines over limit (" STRINGFY_VALUE(MAX_LINES) ")");
	}
	else if (columns > MAX_COLUMNS)
	{
		puts("Failed to create new " ICA_TITLE ": columns over limit (" STRINGFY_VALUE(MAX_COLUMNS) ")");
	}
	else
	{
		ICA_delete();
		
		ICA.lines = lines;
		ICA.columns = columns;
		ICA.matrix = malloc(sizeof(struct ICA_cell) * lines * columns);
	}
}

void ICA_delete(void)
{
	if (ICA.matrix != NULL) // Limpa memoria se já houver um ICA
	{
		
	}
}

void ICA_run(int64_t cycles, int64_t steps, float q)
{
	int line, column;
	struct ICA_cell *cell;
	float delta_q;

	for(; cycles; --cycles)
	{
		for(; steps; --steps)
		{
			line = rand() % ICA.lines;
			column = rand() % ICA.columns;

			cell = ICA_getCell(line, column);

			delta_q = q * (rand() % Q_PRECISION) / Q_PRECISION;

			if (ICA_neighborSum(line, column) > cell->threshold)
			{
				cell->state = 1;
				cell->threshold += delta_q;
			}
			else
			{
				cell->state = -1;
				cell->threshold -= delta_q;
			}
		}
	}
}

unsigned int ICA_start(void)
{
	unsigned int seed = (unsigned int) time(NULL);
	srand(seed);

	for (int32_t line = 0; line < ICA.lines; ++line)
	{
		for (int32_t column = 0; column < ICA.columns; ++column)
		{
			int8_t state = (rand() % 2) * 2 - 1; // +1 or -1
			ICA_set_state(line, column, state);
			ICA_set_threshold(line, column, 0);
		}
	}

	return seed;	
}


int8_t ICA_neighborSum(long line, long column)
{
	return 0;
}

struct ICA_cell *ICA_getCell(long line, long column)
{
	return NULL;
}

void ICA_set_state(long line, long column, int8_t state)
{
	return;
}

void ICA_set_threshold(long line, long column, int8_t state)
{
	return;
}
