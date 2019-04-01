#include <stdlib.h>
#include "printTitle.h"
#include "optionManager.h"

int main(void)
{
	char option;

	printTitle();

	do
	{
		// Menu
		listOptions();
		option = readOption();
		interpretOption(option);

	} while (1);

	return EXIT_FAILURE;
}
