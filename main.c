/*
 * File: proj1.c
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
	/* input - contains the user's input; size == MAX_TASKL -> biggest supported length */
	char input[MAX_TASKL];

	/* noTasks - amount of tasks currently in the system */
	int noTasks = 0;
	/* infoTasks - contains the tasks in the system's data */
	char infoTasks[MAX_TASK][MAX_TASKL];
	
	/* noUsers - amount of users currently in the system */
	int noUsers = 0;
	/* infoUsers - contains the users in the system's data */
	char infoUsers[MAX_USER][MAX_USERL];

	/* noAtv - amount of activities currently in the system */
	int noAtv = 3;
	/* infoAtv - contains the informations in the system's data */
	char infoAtv[MAX_ATV][MAX_ATVL];
	/* "built-in" activities */ 
	infoAtv[0] = "TO DO";
	infoAtv[1] = "IN PROGRESS";
	infoAtv[2] = "DONE";	

	
	do{
		fgets(input, MAX_TASKL, stdin);
		switch(input[0]){
			case 'q':
				state = 1;
				break;
			case 't':
				add_task(input, noTasks);
				break;
			case 'l':
				list_tasks(input, noTasks);
				break;
			/*case 'n': problem for later me */

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

