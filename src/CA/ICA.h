#ifndef ICA_H
#define ICA_H

#include <stdint.h>

#define ICA_TITLE "Inhomogenous Cellular Automata (ICA)"
#define ICA_AUTHOR "Gerard Weisbuch and Dietrich Stauffer"

/*
ICA_new:
TODO: Function description.
*/
void ICA_new(long lines, long columns);

/*
ICA_delete:
TODO: Function description.
*/
void ICA_delete(void);

/*
ICA_run:
TODO: Function description.
*/
void ICA_run(int64_t cycles, int64_t steps, float q);

/*
ICA_start:
TODO: Function description.
*/
unsigned int ICA_start(void);


#endif
