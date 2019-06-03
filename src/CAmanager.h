#ifndef CAMANAGER_H
#define CAMANAGER_H

#include <stdint.h>

void newMatrix(void);
void startSimulationVonNeumann(void);
void startSimulationMoore(void);
void analyse(void);
void render(void);
void drawState(void);
void drawThres(void);

#endif
