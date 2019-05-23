#ifndef ICA_H
#define ICA_H

#include <stdint.h>

#define ICA_TITLE "Inhomogenous Cellular Automata (ICA)"
#define ICA_AUTHOR "Gerard Weisbuch and Dietrich Stauffer"

int ICA_getL(void);
float ICA_getQ(void);
float ICA_getAvgState(void);
float ICA_getAvgThres(void);
unsigned int ICA_getSeed(void);
int ICA_getNumberOfClusters(void);
void ICA_new(int L, float q);
void ICA_setSeed(unsigned int newSeed);
void ICA_newSeeded(int L, float q, unsigned int newSeed);
/*
ICA_invertCell:
inverts the state and threshold of the cell at position (x,y),
where x,y is in the interval of integers [0,L).
*/
void ICA_invertCell(int x, int y);
void ICA_delete(void);
void ICA_run(int32_t cycles, int32_t steps);
void ICA_updateStats(void);

#endif
