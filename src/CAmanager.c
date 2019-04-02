#include "CAmanager.h"

typedef enum matrix_t {ICA} matrix_t;

typedef struct property
{
	const char *name;
	double (*getValue)(void);
	void (*setValue)(double);
	struct property *next;
} property;

struct matrix
{
	matrix_t type;
	int width;
	int height;

	property *properties;

	void (*new)(void);
	void (*delete)(void);
	void (*run)(uint64_t steps);
};


void newMatrix(void)
{

}

void startSimulation(void)
{

}
