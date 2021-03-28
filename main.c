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

	char input[MAX_LENGTH];
	/* c will store the command's characters */
	char c;

	/* stores the program's state - at 0 the program keeps running, at 1 it stops */
	int state = 0;
	int i;

	strcpy(atvProp[TODO].desc, "TO DO");
	atvProp[TODO].noTasks = 0;
	strcpy(atvProp[INPROGRESS].desc, "IN PROGRESS");
	atvProp[INPROGRESS].noTasks = 0;
	strcpy(atvProp[DONE].desc, "DONE");
	atvProp[DONE].noTasks = 0;
	amAtvs += 3;

	do{
		fgets(input, MAX_LENGTH, stdin);
		/* command character might not be the first character in the input */
		for(i = 0; input[i] < 'a' || input[i] > 'z'; i++);
		c = input[i];
		switch(c){
			case 'q':
				state = 1;
				break;
			case 't':
				addTask(input);
				break;
			case 'l':
				listTasks(input);
				break;
			case 'n':
				advance(input);
				break;
			case 'u':
				addUser(input);
				break;
			case 'a':
				addActivity(input);				
			default:
				break;	
		}		
		/* clear the input array for the next stream of input */
		strcpy(input, "");
	}while(state == 0);

	return 0;
}

/* adds a task to the system - 't' command */
void addTask(char read[]){
	
	int counter = 0, j;
	/* index of the character being read */
	int i = START;
	/* predicted duration */
	int pd = 0;
	/* 0 if there's no duplicate description found, 1 if there is */
	int dup = 0;
	/* contains the currently read character from read[] */
	char c = read[i];
	/* temporary array containing a task's description */
	char temp[MAX_TASKL];

	if(amTasks == MAX_TASK){
    	printf(T_TOOMANY);
	}
	
	else{

		while(c != ' ' && c != '\t'){
			if(c >= '0' && c <= '9'){
		 		pd = pd * 10 + (c - '0');
			}
			i++;
			c = read[i];
		}
		
		i++; /* to account for the space between <duration> and <description> */
		c = read[i];

		while(c != '\0' && c != '\n' && c != EOF && counter < MAX_TASKL){
			temp[counter] = c;
			counter++;
			i++;
			c = read[i];
		}

		for(j = 0; j < amTasks && dup == 0; j++){
			if (strcmp(temp, taskProp[j].desc) == 0){
				dup = 1;
			}
		}

		if(dup == 1){
			printf(T_EXISTS);
		}

		else{
			/* id always 1 ahead of taskProp's index */
			strcpy(taskProp[amTasks].desc, temp);
			taskProp[amTasks].pd = pd;
			taskProp[amTasks].st = currentTime;
			/* id always 1 ahead of amTasks */
			taskProp[amTasks].id = amTasks + 1;

			/* the task always starts in the TO DO activity */
			strcpy(taskProp[amTasks].currAtv, atvProp[TODO].desc);
			atvProp[TODO].noTasks++;

			printf(T_WRITEID, taskProp[amTasks].id);
			amTasks++;
		}	
	}
}

/* adds a user to the system/lists all users by creation date - 'u' command */
void addUser(char read[]){
	int counter = 0, j;
	int i = START;
    /* 0 if there's no duplicate description found, 1 if there is */
    int dup = 0;
    /* contains the currently read character from read[] */
    char c = read[i];
    /* temporary array containing a user's description */
    char temp[MAX_USERL];
	/* while at 0, temp doesn't store characters */
	int space = 0;

        while(c != '\0' && c != '\n' && c != EOF && (c != ' ' || space == 0) && counter < MAX_TASKL){
        	if(space == 0){
				if(c != '0'){
					space = 1;
					temp[counter] = c;
					counter++;
					i++;
					c = read[i];
				}
			}
			else{
				temp[counter] = c;
            	counter ++;
        		i++;
          		c = read[i];
			}
		
        }

        for(j = 0; j < amUsers && dup == 0; j++){
        	if (strcmp(temp, userProp[j].desc) == 0){
                	dup = 1;
                }
      	}

        if(dup == 1){
        	printf(U_EXISTS);
        }
	else if(amUsers == MAX_USER && strcmp(temp, "") != 0){
		printf(U_TOOMANY);
	}
    else if(strcmp(temp, "") != 0){
    	strcpy(userProp[amUsers].desc, temp);
        amUsers++;
 	}
	else{
		for(j = 0; j < amUsers; j++){
			printf("%s\n", userProp[j].desc);
		}
	}
}


/* adds an activity to the system/lists all activities by creation date - 'a' command */
void addActivity(char read[]){
	int counter = 0, j, min = 0;
    int i = START;
    /* 0 if there's no duplicate description found, 1 if there is */
    int dup = 0;
    /* contains the currently read character from read[] */
    char c = read[i];
    /* temporary array containing an activity's description */
    char temp[MAX_ATVL];

    while(c != '\0' && c != '\n' && c != EOF && counter < MAX_ATVL){
		if(c >= 'a' && c <= 'z'){
			min = 1;
		}
    	temp[counter] = c;
        counter ++;
        i++;
        c = read[i];
    }

    for(j = 0; j < amAtvs && dup == 0; j++){
    	if (strcmp(temp, atvProp[j].desc) == 0){
        	dup = 1;
        }
    }

    if(dup == 1){
    	printf(A_EXISTS);
    }
	else if(min == 1){
		printf(A_INVALID);
	}
    else if(amAtvs == MAX_ATV && strcmp(temp, "") != 0){
    	printf(A_TOOMANY);
    }
    else if(strcmp(temp, "") != 0){
    	strcpy(atvProp[amAtvs].desc, temp);
        amAtvs++;
    }
    else{
    	for(j = 0; j < amAtvs; j++){
        	printf("%s\n", atvProp[j].desc);
        }
    }

}

/* advances the system's time - 'n' command*/
void advance(char read[]){
	
	int i = START;
	int time = 0;
	char c = read[i];
	int neg = 0;

	while(c != '\0' && c != '\n' && c != ' ' && c != EOF){
		if(c == '-'){
			neg = 1;
		}
		else{
			if(c >= '0' && c <= '9'){
				time = time * 10 + (c - '0');
				i++;
				c = read[i];
			}
		}
	}

	if(neg != 1){
		if(time != 0){
			currentTime += time;
		}

		printf("%d\n", currentTime);
	}
	else{
		printf(TIME_INVALID);
	}
}

void listTasks(char read[]){
	int i, j = START, changed = 1, space = 0;
	int idTemp = 0, idCount = 0;
	int idWrong = 0;
	int cap = amTasks;
	task ordered[MAX_TASK], taskArray[MAX_TASK], tempTask;
	char c = read[j];

	while(c != '\0' && c != '\n' && c != EOF && idWrong == 0){
		if(space == 0 && (c == '\t' || c == ' ')){
			space = 1;
			idWrong = 1;
			for(i = 0; i < amTasks && idWrong == 1; i++){
				if (taskProp[i].id == idTemp){
					idWrong = 0;
					taskArray[idCount] = taskProp[i];
					idTemp = 0;
					idCount++;
				}
			}
		}
		else if(c >= '0' && c <= '9'){
			if(space == 1){
				space = 0;
			}
			idTemp = idTemp * 10 + (c - '0');
		}

		j++;
		c = read[j];

	}

	idWrong = 1;
	for(i = 0; i < amTasks && idWrong == 1; i++){
		if (taskProp[i].id == idTemp){
			idWrong = 0;
			taskArray[idCount] = taskProp[i];
			idTemp = 0;
			idCount++;
		}
	}

	if(idWrong == 1 && idTemp != 0){
		printf(T_NOID, idTemp);
	}

	else{
		if(idCount > 0){
			for(i = 0; i < idCount; i++){
				printf("%d %s #%d %s\n", \
						taskArray[i].id, \
						taskArray[i].currAtv, \
						taskArray[i].pd, \
						taskArray[i].desc \
						);
			}
		}

		else{
			memcpy(ordered, taskProp, sizeof(taskProp));

			while(changed == 1){
				changed = 0;
				for(i = 0; i < cap - 1; i++){
					if(strcmp(ordered[i].desc, ordered[i+1].desc) > 0){
						tempTask = ordered[i];
						ordered[i] = ordered[i+1];
						ordered[i+1] = tempTask;
						changed = 1;
					}
				}
				cap--;
			}
			for(i = 0; i < amTasks; i++){
				printf("%d %s #%d %s\n", \
						ordered[i].id, \
						ordered[i].currAtv, \
						ordered[i].pd, \
						ordered[i].desc \
						);
			}	
		}
	}

}