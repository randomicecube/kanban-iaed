/*
 * File: main.c
 * Author: Diogo Gaspar, 99207 - LEIC-A
 * Description: Main program file
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "util.h"

int main(){

	/* state - while at 0 the program keeps running, if at 1 the program stops */
	int state = 0;
	/* input - contains the user's input; size == MAX_LENGTH -> biggest supported length */
	char input[MAX_LENGTH];
	/* currentTime - current time */
	int currentTime = 0;	

	
	do{
		fgets(input, MAX_LENGTH, stdin);
		switch(input[0]){
			case 'q':
				state = 1;
				break;
			case 't':
				add_task(input, noTasks, infoTasks);
				break;
			case 'l':
				list_tasks(input, noTasks);
				break;
			/*case 'n': problem for a-bit-from-now me */

			case 'u':
				addl_users(input, noUsers);
				break;
			case 'm':
				move_task(input, noUsers, noTasks, noActivities);
				break;
			case 'd':
				list_t_atv(input);
				break;
			case 'a':
				add_atv(input, noAtv);
				break;
			default:
				break;	
					
		}		

		/* clear the input array for the next stream of input */
		strcpy(input, "");
	}while(state == 0);

	return 0;
}
