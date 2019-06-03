#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "optionManager.h"
#include "CAmanager.h"

char readOption(void)
{
	char ch;

	while (isblank(ch = getchar()));
	
	if (ch == '\n' || ch == EOF)
		return 0;
	
	while(getchar() != '\n');

	return toupper(ch);
}

void listOptions(void)
{
	puts("Please enter one of the options below.");
	puts("Simulation:");
	puts("\t[N]ew matrix | [S]tart simulation | [Q]uit");
	puts("Settings:");
	puts("\t[A]nalyse Matrix");
	//printf("\tPrinting "); analysis_printActiveFeatures(); printf(" every n cycles and m steps.\n");
	puts("\t[R]ender Scatter Chart");
	//puts("\tActive charts:"); chart_printActiveCharts();
	puts("\t[D]raw matrix");
	//puts("\tActive drawings:"); draw_printActiveDrawings();
	puts("\t[W]rite matrix to hard drive (Unimplemented)");
	//printf("Saving matrix every %d cycles and %d steps.\n");
	puts("\t[C]reate data files. (Unimplemented)");
	//printf("Saving data every %d cycles and %d steps to file \"%s\".\n",)
}

void interpretOption(char option)
{
	char subOption;
	
	switch(option)
	{
		case 'N':
		newMatrix();
		break;

		case 'S':
		do
		{
			printf("Which Neighborhood? (M)oore | (V)onNeumann | (C)ancel: ");
			subOption = getchar();
			while(getchar() != '\n');
			if(toupper(subOption) == 'M')
			{
				startSimulationMoore();
				break;
			}
			if(toupper(subOption) == 'V')
			{
				startSimulationVonNeumann();
				break;
			}
			if(toupper(subOption) == 'C')
				break;
		} while(1);
		break;

		case 'A':
		analyse();
		break;

		case 'R':
		render();
		break;

		case 'D':
		do
		{
			printf("Draw (S)tate or (T)hreshold | (C)ancel: ");
			subOption = getchar();
			while(getchar() != '\n');
			if(toupper(subOption) == 'S')
			{
				drawState();
				break;
			}
			if(toupper(subOption) == 'T')
			{
				drawThres();
				break;
			}
			if(toupper(subOption) == 'C')
				break;
		} while(1);
		break;

		case 'W':
		puts("Unimplemented");
		break;

		case 'C':
		puts("Unimplemented");
		break;

		case 'Q':
		exit(EXIT_SUCCESS);
		break;

		default:
		puts("Invalid option.");
		break;
	}
}
