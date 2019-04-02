#ifndef ICA_H
#define ICA_H

#include <stdint.h>

#define ICA_TITLE "Inhomogenous Cellular Automata (ICA)"
#define ICA_AUTHOR "Gerard Weisbuch and Dietrich Stauffer"

void ICA_new(int L);
void ICA_delete(void);
void ICA_run(int32_t cycles, int32_t steps);
void ICA_updateStats(void);

#endif
