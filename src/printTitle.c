#include "printTitle.h"
#include <stdio.h>

static void printSeparator(int n)
{
	for (int i = 0; i < n; ++i)
		putchar('~');
	putchar('\n');
}

void printTitle(void)
{
	puts(TITLE);
	puts("Written by " AUTHOR);
	puts(INSTITUTION);
	puts("Written in " DATE ". " "Compiled on " __TIME__ " " __DATE__);
	//puts("Compiled on " __TIME__ " " __DATE__);

	putchar('\n');
}
