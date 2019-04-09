#ifndef COUNTCLUSTER_H
#define COUNTCLUSTER_H

#include <stdint.h>

typedef struct neighbor_t {void *N, *S, *E, *W;} neighbor_t;

typedef struct matrix_t 
{
	void *m; 
	int rows; 
	int cols;
	int (*getCellState)(void *c);
	void *(*getCell)(int row, int col, const struct matrix_t *m);
	neighbor_t *(*getNeighbor)(void *c, const struct matrix_t *m);
	uint64_t (*getPos)(void *c, const struct matrix_t *m);
} matrix_t;

/*
countClusters:
Counts the number of clusters using a contamination algorithm. Receives
an object of type matrix_t which must contain the matrix to be analysed, 
the number of columns and row, and functions for getting a cell's state,
getting a cell given a position, getting the neighborhood of a given cell,
and getting the position of a given cell in the given matrix.
*/
int countClusters(const matrix_t *m);

#endif
