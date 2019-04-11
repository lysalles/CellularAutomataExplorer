#include "countCluster.h"
#include <stdbool.h>
#include <stdlib.h>

int countClusters(const matrix_t *m) 
{
	// IMPLEMENTATION INDEPENDENT
	int maxRows = m->rows, maxCols = m->cols;

	int (*getCellState)(void *c) = m->getCellState;
	void *(*getCell)(int row, int col, const matrix_t *m) = m->getCell;
	neighbor_t *(*getNeighbor)(void *c, const matrix_t *m) = m->getNeighbor;
	uint64_t (*getPos)(void *c, const matrix_t *m) = m->getPos;
	
	bool * const cellLooked = calloc((maxRows + 2) * (maxCols + 2), sizeof(bool));
	int numberOfClusters = 0;
	
	void *N, *S, *E, *W, *C;
	bool newCluster;
	int row, col;
	neighbor_t *neighbor;

	// queue
	int queueSize = 2 * maxCols + 2 * maxRows;
	void **queue = calloc(queueSize, sizeof(void *));
	int first = 0, last = 0, elements = 0;
	#define ENQUEUE(x) *(queue + last) = x; last = last + 1 < queueSize ? last + 1 : 0; ++elements
	#define DEQUEUE *(queue + first); first = first + 1 < queueSize ? first + 1 : 0; --elements

	// ALGORITHM
	for (row = 1; row <= maxRows; ++row)
		for (col = 1; col <= maxCols; ++col)
		{
			C = getCell(row, col, m);

			if (getCellState(C) != 1 || *(cellLooked + getPos(C, m)) == true)
				continue;

			newCluster = false;
			*(cellLooked + getPos(C, m)) = true;
			ENQUEUE(C);

			while (elements > 0)
			{
				C = DEQUEUE;

				neighbor = getNeighbor(C, m);
				N = neighbor->N;
				S = neighbor->S;
				E = neighbor->E;
				W = neighbor->W;
				
				if (getCellState(N) == 1 && *(cellLooked + getPos(N, m)) == false)
				{
					*(cellLooked + getPos(N, m)) = true;
					newCluster = true;
					ENQUEUE(N);
				} 
				if (getCellState(S) == 1 && *(cellLooked + getPos(S, m)) == false)
				{
					*(cellLooked + getPos(S, m)) = true;
					newCluster = true;
					ENQUEUE(S);
				} 
				if (getCellState(E) == 1 && *(cellLooked + getPos(E, m)) == false)
				{
					*(cellLooked + getPos(E, m)) = true;
					newCluster = true;
					ENQUEUE(E);
				} 
				if (getCellState(W) == 1 && *(cellLooked + getPos(W, m)) == false)
				{
					*(cellLooked + getPos(W, m)) = true;
					newCluster = true;
					ENQUEUE(W);
				} 
			}

			if (newCluster) ++numberOfClusters;
		}
		
	free(queue);
	free(cellLooked);

	return numberOfClusters;
}
