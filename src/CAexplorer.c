#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "printTitle.h"
#include "optionManager.h"

int main(void)
{
	srand((unsigned int) time(NULL));
	char option;

	printTitle();

	do
	{
		// Menu
		listOptions();
		printf("Option: ");
		option = readOption();
		interpretOption(option);
		putchar('\n');

	} while (1);

	return EXIT_FAILURE;
}
