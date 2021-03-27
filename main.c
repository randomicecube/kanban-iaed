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

	strcpy(atv[0], "TO DO");
	strcpy(atv[1], "IN PROGRESS");
	strcpy(atv[2], "DONE");

	do{
		fgets(input, MAX_LENGTH, stdin);
		switch(input[0]){
			case 'q':
				state = 1;
				break;
			case 't':
				addTask(input);
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

	if(amounts[0] == MAX_TASK){
    	printf("too many tasks\n");
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
			counter ++;
			i++;
			c = read[i];
		}

		for(j = 0; j < amounts[0] && dup == 0; j++){
			if (strcmp(temp, dTask[j]) == 0){
				dup = 1;
			}
		}

		if(dup == 1){
			printf("duplicate description\n");
		}

		else{
			strcpy(dTask[amounts[0]], temp);
			pTask[amounts[0]][0] = pd;
			pTask[amounts[0]][1] = currentTime;
			pTask[amounts[0]][2] = amounts[0] + 1; /* id always 1 ahead of index */
			printf("task %d\n", pTask[amounts[0]][2]);
			amounts[0]++;
		}	
	}
}

/* adds a user to the system - 'u' command*/
void addUser(char read[]){
	int counter = 0, j;
	int i = START;
        /* 0 if there's no duplicate description found, 1 if there is */
        int dup = 0;
        /* contains the currently read character from read[] */
        char c = read[i];
        /* temporary array containing a user's description */
        char temp[MAX_TASKL];
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

        for(j = 0; j < amounts[1] && dup == 0; j++){
        	if (strcmp(temp, user[j]) == 0){
                	dup = 1;
                }
      	}

        if(dup == 1){
        	printf("user already exists\n");
        }
	else if(amounts[1] == MAX_USER && strcmp(temp, "") != 0){
		printf("too many users\n");
	}
    else if(strcmp(temp, "") != 0){
    	strcpy(user[amounts[1]], temp);
        amounts[1]++;
 	}
	else{
		for(j = 0; j < amounts[1]; j++){
			printf("%s\n", user[j]);
		}
	}
}


/* adds an activity to the system - 'a' command */
void addActivity(char read[]){
	int counter = 0, j, min = 0;
    int i = START;
    /* 0 if there's no duplicate description found, 1 if there is */
    int dup = 0;
    /* contains the currently read character from read[] */
    char c = read[i];
    /* temporary array containing an activity's description */
    char temp[MAX_TASKL];

    while(c != '\0' && c != '\n' && c != EOF && counter < MAX_ATVL){
		if(c >= 'a' && c <= 'z'){
			min = 1;
		}
    	temp[counter] = c;
        counter ++;
        i++;
        c = read[i];
    }

    for(j = 0; j < amounts[2] && dup == 0; j++){
    	if (strcmp(temp, atv[j]) == 0){
        	dup = 1;
        }
    }

    if(dup == 1){
    	printf("duplicate activity\n");
    }
	else if(min == 1){
		printf("invalid description\n");
	}
    else if(amounts[2] == MAX_ATV && strcmp(temp, "") != 0){
    	printf("too many activities\n");
    }
    else if(strcmp(temp, "") != 0){
    	strcpy(atv[amounts[2]], temp);
        amounts[2]++;
    }
    else{
    	for(j = 0; j < amounts[2]; j++){
        	printf("%s\n", atv[j]);
        }
    }

}

/* advances the system's time - 'n' command*/
void advance(char read[]){
	
	int i = START;
	int time = 0;
	char c = read[i];
	int neg = 0;

	while(c != '\0' && c != '\n' && c != ' ' && c != EOF && state == 0){
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
		printf("invalid time\n");
	}
}
