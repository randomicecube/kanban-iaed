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

/* main program's function */
int main(){
	char input[MAX_LENGTH];
	int state = 0; /* at 0 the program keeps running, at 1 it stops */

	/* starting the 3 initial activities */
	strcpy(atvProp[TODO].desc, S_TODO);
	atvProp[TODO].noTasks = 0;
	strcpy(atvProp[INPROGRESS].desc, S_INPROGRESS);
	atvProp[INPROGRESS].noTasks = 0;
	strcpy(atvProp[DONE].desc, S_DONE);
	atvProp[DONE].noTasks = 0;
	amAtvs += 3; /* activity count after adding the 3 atvs above*/

	do{
		fgets(input, MAX_LENGTH, stdin);
		switch(input[0]){
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
			case 'm':
				moveTasks(input);
				break;
			case 'd':
				listAtvTasks(input);
				break;
			case 'a':
				addActivity(input);				
			default:
				break;	
		}
		strcpy(input, ""); /* clear the input array for the next input */
	}while(!state);

	return 0;
}

/* adds a task to the system - 't' command */
void addTask(char read[]){
	int index; /* index of the character being read */
	int pd = 0; /* predicted duration */
	char temp[MAX_TASKL] = {'\0'};

	if(amTasks == MAX_TASK){ /* the system cannot accept more tasks */
    	printf(T_TOOMANY);
		return;
	}
	
	pd = readNumber(read, START); /* reading the predicted duration */
	index = getNextIndex(read, START);

	readTaskAtv(read, temp, index, MAX_TASKL); /* reading description */	
	if(dupTask(temp, taskProp, amTasks) == 1){ /* if it already exists */
		printf(T_EXISTS);
		return;
	}
	else if(pd < 0){ /* if the duration is a negative integer */
		printf(INV_DUR);
	}

	/* adding the new task's information into the task array */
	strcpy(taskProp[amTasks].desc, temp);
	taskProp[amTasks].pd = pd;
	taskProp[amTasks].st = 0;
	taskProp[amTasks].duration = 0;
	taskProp[amTasks].id = amTasks + 1; /* id always 1 ahead of amTasks */

	/* tasks always start in the TO DO activity */
	strcpy(taskProp[amTasks].currAtv, atvProp[TODO].desc);
	atvProp[TODO].noTasks++;

	printf(T_WRITEID, taskProp[amTasks].id);
	amTasks++;
	return;
}

/* adds a user to the system/lists all users by creation date - 'u' command */
void addUser(char read[]){
	int j;
    char temp[MAX_USERL] = {'\0'};
	
	readUser(read, temp, START, MAX_USERL); /* reading description */
    if(dupUser(temp, userProp, amUsers) == 1){ /* if it already exists */
        printf(U_EXISTS);
    }
	else if(amUsers == MAX_USER){ /* if the system can't accept more users */
		printf(U_TOOMANY);
	}
	else if(strcmp(temp, "") != 0){ /* if the system is adding a user */
    	strcpy(userProp[amUsers].desc, temp);
        amUsers++;
 	}
	else{ /* if the request is to print all usernames */
		for(j = 0; j < amUsers; j++){
			printf("%s\n", userProp[j].desc);
		}
	}
	return;
}


/* adds an activity to the system/lists all activities - 'a' command */
void addActivity(char read[]){
	int j;
    char temp[MAX_ATVL] = {'\0'};

	readTaskAtv(read, temp, START, MAX_ATVL);
	/* checks if the description is in the system */
    for(j = 0; j < amAtvs; j++){ 
    	if (strcmp(temp, atvProp[j].desc) == 0){
        	printf(A_EXISTS);
			return;
        }
    }
	/* checks if there are lowercase characters */
    for(j = 0; j < (int) strlen(temp); j++){ 
		if(islower(temp[j])){
			printf(A_INVALID);
			return;
		}
	}

    if(amAtvs == MAX_ATV && strcmp(temp, "") != 0){ 
    	printf(A_TOOMANY); /* if the system can't accept more atvs */
    }
	else if(strcmp(temp, "") != 0){ 
    	strcpy(atvProp[amAtvs].desc, temp); /* add activity */
        amAtvs++;
    }
    else{ 
		/* list all activities */
    	for(j = 0; j < amAtvs; j++){ 
        	printf("%s\n", atvProp[j].desc);
        }
    }
	return;
}

/* advances the system's time - 'n' command*/
void advance(char read[]){
	int i = START;
	int time = 0;
	int reading = 0; /* 0: hasn't found a whitespace; 1: found it */
	char c = read[i];

	while(COND && (!isspace(c) || reading == 0)){
		if(c == '-' || c == '.'){ /* if a negative/float was in the input */
			printf(TIME_INVALID);
			return;
		}
		else{
			if(reading == 0){ /* a digit is being read for the first time */
				reading = 1;
			}
			time = time * 10 + (c - '0');
		}
		i++;
		c = read[i];
	}
	currentTime += time;
	printf("%d\n", currentTime);
	
	return;
}

/* lists all the tasks/a subset of tasks in the system - 'l' command */
void listTasks(char read[]){
	int j = START;
	int any; /* FAIL: no matching id; otherwise, its index in the task array */
	int idTemp = 0;
	int idCount = 0; /* amount of ID's read in the input */
	int cap = amTasks; /* how many passes the loop has to do in bubble */
	int error = 0; /* how many invalid IDs were in the input */
	task ordered[MAX_TASK] = {0};
	char c = read[j];

	while(COND){
		idTemp = readNumber(read, j);
		j = getNextIndex(read, j);
		any = anyId(idTemp, amTasks, taskProp);
		if(any == FAIL){ /* if the ID is not in the system */
			printf(T_NOID, idTemp);
			error++;
		}
		else{
			/* prints the valid inputs' information */
			printf("%d %s #%d %s\n", 
					taskProp[any].id, \
					taskProp[any].currAtv, \
					taskProp[any].pd, \
					taskProp[any].desc
					);
			idCount++;
		}
		c = read[j];
	}

	if(error == 0 && idCount == 0){ /*if the input was literally 'l' */
		memcpy(ordered, taskProp, sizeof(taskProp));
		/* sort all the tasks in the system alphabetically (by description) */
		bubble(ordered, cap, LT);
		printTasks(ordered, amTasks);
	}
	return;
}

/* prints the tasks currently in a given activity's scope */
void listAtvTasks(char read[]){
	int i, j = 0, index = 0;
	int found = 0; /* stores whether the activity is in the system or not */
	int cap; /* how many passes the loop has to do in bubble */
	char activity[MAX_ATVL] = {'\0'};
	task ordered[MAX_TASK] = {0};
	atv wanted;

	readTaskAtv(read, activity, START, MAX_ATVL);
	/* checks if the activity is in the system */
	for(j = 0; j < amAtvs && found == 0; j++){ 
		if(!strcmp(atvProp[j].desc, activity)){ /* if it is */
			found = 1;
			wanted = atvProp[j];
			cap = wanted.noTasks;
		}
	}
	if(found == 0){ /* if the activity isn't in the system */
		printf(A_NOTFOUND);
		return;
	}
	/* stores the tasks in the activity's scope in a to-be-sorted array */
	for(i = 0; i < amTasks; i++){
		if(strcmp(taskProp[i].currAtv, wanted.desc) == 0){
			ordered[index] = taskProp[i];
			index++;
		}
	}
	bubble(ordered, cap, LAT);
	for(j = 0; j < wanted.noTasks; j++){
		printf("%d %d %s\n", ordered[j].id, ordered[j].st, ordered[j].desc);
	}
	return;
}

/* moves a Task from an activity to the other; 'm' command */
void moveTasks(char read[]){
	int i = START, idTemp = 0, afterAtv, beforeAtv, afterTask;
	int wrongUser = 0, wrongAtv = 0;
	char username[MAX_USERL] = {'\0'}, atvDesc[MAX_ATVL] = {'\0'};
	char temp[MAX_ATVL] = {'\0'};
	
	idTemp = readNumber(read, i);
	i = getNextIndex(read, i);

	if(anyId(idTemp, amTasks, taskProp) == FAIL){ 
		printf(NO_ID_M); /* if the ID isn't in the system */
		return;
	}

	readUser(read, username, i, MAX_USERL);
	i = getNextIndex(read, i);
	if(dupUser(username, userProp, amUsers) == ZERO){ 
		wrongUser = 1; /* if the username isn't in the system */
	}

	readTaskAtv(read, atvDesc, i, MAX_ATVL);
	if(dupAtv(atvDesc, atvProp, amAtvs) == ZERO){ 
		wrongAtv = 1; /* if the activity isn't in the system */
	}

	if(strcmp(atvDesc,S_TODO) == 0){ 
		printf(T_STARTED); /* if the "going to" activity is TO DO */
		return;
	}

	if(wrongUser == 1){ 
		printf(U_NOTFOUND); /* if the username is not in the system */
		return;
	}

	if(wrongAtv == 1){ 
		printf(A_NOTFOUND); /* if the activity is not in the system */
		return;
	}
	
	/* checks in the system which is the task with the given id; stores it */
	for(i = 0; i < amTasks; i++){
		if(taskProp[i].id == idTemp){
			afterTask = i;
		}
	}

	strcpy(temp, taskProp[afterTask].currAtv);
	for(i = 0; i < amAtvs; i++){
		if(strcmp(atvDesc, atvProp[i].desc) == 0){ 
			afterAtv = i; /* if the "going to" activity is found */
		}

		if(strcmp(temp, atvProp[i].desc) == 0){ 
			beforeAtv = i; /* if the "activity being left" is found */
		}
	}

	/* adjusts the number of tasks in the activities found above */	
	atvProp[beforeAtv].noTasks--;
	atvProp[afterAtv].noTasks++;

	strcpy(taskProp[afterTask].currAtv, atvDesc);

	/* if the "activity being left" is TO DO, the starting time is adjusted */
	if(strcmp(temp, S_TODO) == 0){
		taskProp[afterTask].st = currentTime;		
	}
	else{ /* else, the duration is adjusted */
		taskProp[afterTask].duration = currentTime - taskProp[afterTask].st;
	}

	taskProp[afterTask].slack = taskProp[afterTask].duration - taskProp[afterTask].pd;

	/* only prints if the "activity being left" is not DONE and the "going to" is */
	if(strcmp(atvDesc, S_DONE) == 0 && strcmp(temp, S_DONE) != 0){
		printf(DURSLACK, taskProp[afterTask].duration, taskProp[afterTask].slack);
	}
	return;
}

/* ------------------------------AUX FUNCTIONS------------------------------ */

/* FAIL if the ID isn't in the system; its index in the tasks array if it is */
int anyId(int n, int size, task v[]){
	int i, res = FAIL;
	for(i = 0; i < size; i++){
		if(v[i].id == n){
			res = i;
		}
	}
	return res;
}


/* reads a portion of a string and returns the first integer found*/
int readNumber(char v[], int start){
	int i = start, reading = 0, res = FAIL; 
	int menos = 0;
	char c = v[i];
	while(reading == 0 || !isspace(c)){
		if(reading == 0 && !isspace(c)){
			res = 0;
			reading = 1;
		}
		if(isdigit((int) c)){
		 	res = res * 10 + (c - '0');
		}
		if(c == '-'){
			menos = 1;
		}
		i++;
		c = v[i];
	}
	if(menos == 1 && res != FAIL){
		res = -res;
	}
	return res;
}

/* returns the  v's index after the whitespace found after a number/word */
int getNextIndex(char v[], int start){
	int i = start, reading = 0;
	char c = v[i];
	while(reading == 0 || !isspace(c)){
		if(reading == 0 && !isspace(c)){
			reading = 1;
		}
		i++;
		c = v[i];
	}
	i++;
	return i;
}

/* reads a portion of a string and stores the description in s */
void readTaskAtv(char v[], char *s, int start, int max){ 
	int i = start, index = 0, reading = 0;
	char c = v[i];
	while(COND && index < max){
		if (reading == 0 && !isspace(c)){
			reading = 1;
		}
		if(reading == 1){
			s[index] = c;
			index++;
		}
		i++;
		c = v[i];
	}
	return;
}

/* reads a portion of a string and stores the username in s */
void readUser(char v[], char *s, int start, int max){
	int i = start, index = 0, stop = 0, reading = 0;
	char c = v[i];
	while(COND && index < max && stop == 0){
		if(reading == 0 && !isspace(c)){
			reading = 1;
		}
		if(reading == 1){
			if(isspace(c)){
				stop = 1;
			}
			else{
				s[index] = c;
				index++;
			}
		}
		i++;
		c = v[i];
	}
	return;
}

/* used in listTasks to print the tasks' information */
void printTasks(task v[], int n){
	int i;
	for(i = 0; i < n; i++){
		printf("%d %s #%d %s\n", v[i].id, v[i].currAtv, v[i].pd, v[i].desc);
	}
	return;
}

/* checks if a given (task) description already exists in the system */
int dupTask(char s[], task v[], int amt){
	int i, dup = 0;
	for(i = 0; i < amt; i++){
		if(strcmp(s, v[i].desc) == 0){
			dup = 1;
		}
	}
	return dup;
}

/* checks if a given (user) description already exists in the system */
int dupUser(char s[], user v[], int amt){
	int i, dup = 0;
	for(i = 0; i < amt; i++){
		if(strcmp(s, v[i].desc) == 0){
			dup = 1;
		}
	}
	return dup;
}

/* checks if a given (activity) description already exists in the system */
int dupAtv(char s[], atv v[], int amt){
	int i, dup = 0;
	for(i = 0; i < amt; i++){
		if(strcmp(s, v[i].desc) == 0){
			dup = 1;
		}
	}
	return dup;
}

/* sorts a given array of tasks */
void bubble(task v[], int cap, int func){
	/* func == ONE -> listTasks; func == ZERO -> listAtvTasks */
	int i, changed = 1;
	task temp;
	while(changed == 1){
		changed = 0;
		for(i = 0; i < cap - 1; i++){
			if(v[i].st > v[i + 1].st && func == LAT){
				temp = v[i];
				v[i] = v[i + 1];
				v[i + 1] = temp;
				changed = 1;
			}
			else if(v[i].st == v[i + 1].st || func == LT){
				if(strcmp(v[i].desc, v[i + 1].desc) > 0){
					temp = v[i];
					v[i] = v[i + 1];
					v[i + 1] = temp;
					changed = 1;
				}
			}
		}
		cap--;
	}
	return;
}