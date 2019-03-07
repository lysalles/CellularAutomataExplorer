#include "printTitle.h"
#include <stdio.h>

void printSeparator(int n)
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
	puts(DATE);
	
	printSeparator(33);
	puts("Compiled on " __TIME__ " " __DATE__);
	printSeparator(33);

	putchar('\n');
}
