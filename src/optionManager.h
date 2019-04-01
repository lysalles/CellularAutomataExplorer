#ifndef OPTIONMANAGER_H
#define OPTIONMANAGER_H

/*
readOption:
returns the first nonblank character the user enters. Returns 0 if input is 
invalid
*/
char readOption(void);

/*
listOptions:
lists the options available to the user
*/
void listOptions(void);

/*
interpretOption:
tries to match the option supplied to a predefined option. Prints an error 
message if no match was possible
*/
void interpretOption(char option);

#endif
