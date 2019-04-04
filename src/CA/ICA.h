#ifndef ICA_H
#define ICA_H

#include <stdint.h>

#define ICA_TITLE "Inhomogenous Cellular Automata (ICA)"
#define ICA_AUTHOR "Gerard Weisbuch and Dietrich Stauffer"

float ICA_getL(void);
float ICA_getQ(void);
float ICA_getAvgState(void);
float ICA_getAvgThres(void);
unsigned int ICA_getSeed(void);
int ICA_getNumberOfClusters(void);
void ICA_new(int L, float q);
void ICA_delete(void);
void ICA_run(int32_t cycles, int32_t steps);
void ICA_updateStats(void);

#endif
