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

/* main program function */
int main(){
	char input[MAX_LENGTH];
	start();
	do{
		fgets(input, MAX_LENGTH, stdin);
		redirectCommand(input);
		input[0] = '\0'; /* clear the input array for the next line */
	}while(!state);
	return 0;
}

/* adds a task to the system - 't' command */
void addTask(char read[]){
	int index, pd = 0; 
	char temp[MAX_TASKL] = {'\0'};

	if(amTasks == MAX_TASK){
    	printf(T_TOOMANY);
		return;
	}
	
	pd = readNumber(read, START); 
	index = getNextIndex(read, START);

	readTaskAtv(read, temp, index, MAX_TASKL);	
	if(dupSearch(TASK, temp)){
		printf(T_EXISTS);
		return;
	}
	else if(pd <= 0){
		printf(INV_DUR);
		return;
	}

	updateTaskVec(temp, pd); 
	return;
}

/* adds a user to the system/lists all users by creation date - 'u' command */
void addUser(char read[]){
	int j;
    char temp[MAX_USERL] = {'\0'};
	
	readUser(read, temp, START, MAX_USERL); 
    if(dupSearch(USER, temp)){
        printf(U_EXISTS);
    }
	else if(amUsers == MAX_USER && strcmp(temp, "")){
		printf(U_TOOMANY);
	}
	else if(strcmp(temp, "")){ /* add user */
    	strcpy(userProp[amUsers++].desc, temp);
 	}
	else{
		for(j = 0; j < amUsers; j++){ /* list all users */
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
    if(findErrorAddAtv(temp) == FAIL){
		return;
	}
	else if(strcmp(temp, "")){ 
    	strcpy(atvProp[amAtvs++].desc, temp); /* add activity */
    }
    else{ 
    	for(j = 0; j < amAtvs; j++){ /* list all activities */ 
        	printf("%s\n", atvProp[j].desc);
        }
    }
	return;
}

/* advances the system's time - 'n' command*/
void advanceTime(char read[]){
	int time = readNumber(read, START);
	if(time < 0){
		printf(TIME_INVALID);
		return;
	}
	currentTime += time;
	printf("%d\n", currentTime);
	return;
}

/* lists all the tasks/a subset of tasks in the system - 'l' command */
void listTasks(char read[]){
	int j = START, idTemp, idCount = 0, error = 0, any;
	char c = read[j];

	while(COND(c)){
		idTemp = readNumber(read, j);
		j = getNextIndex(read, j);
		any = anyId(idTemp, amTasks, taskProp);
		if(any == FAIL){ /* if the ID is not in the system */
			printf(T_NOID, idTemp);
			error++;
		}
		else{
			/* prints the input's information if it was valid*/
			printf("%d %s #%d %s\n", taskProp[any].id, taskProp[any].currAtv, \
					taskProp[any].pd, taskProp[any].desc);
			idCount++;
		}
		if(j == FAIL && idCount > 0){
			return; /* in case there are no more indexes to be read */
		}
		c = read[j];
	}

	if(!error && idCount == 0){ /*if the input was literally 'l' */
		shellL(amTasks);
		printList(taskProp, amTasks, LT);
	}
	return;
}

/* prints the tasks currently in a given activity's scope - 'd' command */
void listAtvTasks(char read[]){
	int i, j = 0, index = 0, found = 0;
	char activity[MAX_ATVL] = {'\0'};
	task atvTasks[MAX_TASK] = {0};
	atv wantedAtv;

	readTaskAtv(read, activity, START, MAX_ATVL);
	/* checks if the activity is in the system */
	for(j = 0; j < amAtvs && found == 0; j++){ 
		if(!strcmp(atvProp[j].desc, activity)){
			found++;
			wantedAtv = atvProp[j];
		}
	}
	if(!found){ /* if the activity isn't in the system */
		printf(A_NOTFOUND);
		return;
	}
	/* stores the tasks in the activity's scope in a to-be-sorted array */
	for(i = 0; i < amTasks; i++){
		if(!strcmp(taskProp[i].currAtv, wantedAtv.desc)){
			atvTasks[index++] = taskProp[i];
		}
	}
	bubbleD(atvTasks, wantedAtv.noTasks);
	printList(atvTasks, wantedAtv.noTasks, LAT);
	return;
}

/* moves a Task from an activity to the other; 'm' command */
void moveTasks(char read[]){
	int i, idTemp, afterAtv, beforeAtv, afterTask, wrongUser = 0, wrongAtv = 0;
	char username[MAX_USERL] = {'\0'}, afterDesc[MAX_ATVL] = {'\0'}, temp[MAX_ATVL] = {'\0'};
	
	idTemp = readNumber(read, START);
	i = getNextIndex(read, START);

	readUser(read, username, i, MAX_USERL);
	readTaskAtv(read, afterDesc, getNextIndex(read, i), MAX_ATVL);
	wrongUser = dupSearch(USER, username);
	wrongAtv = dupSearch(ATV, afterDesc);
	afterTask = findIndexTask(taskProp, idTemp);
	strcpy(temp, taskProp[afterTask].currAtv);
	if(printErrorsMove(idTemp, temp, afterDesc, wrongUser, wrongAtv)){
		return; /* returns to main if there were any errors found */
	}
	beforeAtv = findIndexAtv(atvProp, temp);
	afterAtv = findIndexAtv(atvProp, afterDesc);
	update_printMove(beforeAtv, afterAtv, afterTask, temp, afterDesc);
	return;
}

/* ------------------------------AUX FUNCTIONS------------------------------ */

/* starts the 3 "default" activities */
void start(){
	strcpy(atvProp[TODO].desc, S_TODO);
	atvProp[TODO].noTasks = 0;
	strcpy(atvProp[INPROGRESS].desc, S_INPROGRESS);
	atvProp[INPROGRESS].noTasks = 0;
	strcpy(atvProp[DONE].desc, S_DONE);
	atvProp[DONE].noTasks = 0;
	amAtvs += 3;
	return;
}

/* redirects the program to a specific function, depending on the command */
void redirectCommand(char input[]){
	switch(input[0]){ /* we know the command character is always the first */
		case 'q':
			state++;
			break;
		case 't':
			addTask(input);
			break;
		case 'l':
			listTasks(input);
			break;
		case 'n':
			advanceTime(input);
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
			addActivity(input); /* default not needed, command always right */
	}
	return;
}

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
	char c = v[i++];
	while(reading == 0 || !isspace(c)){
		if(reading == 0 && !isspace(c)){ /* the input will now be stored */
			res = 0;
			reading = 1;
		}
		if(isdigit((int) c)){
		 	res = res * 10 + (c - '0');
		}
		else if(c == '-'){
			menos = 1;
		}
		c = v[i++];
	}
	if(menos == 1 && res != FAIL){
		res = -res;
	}
	return res;
}

/* reads a portion of a string and stores the description in s */
void readTaskAtv(char v[], char *s, int start, int max){ 
	int i = start, index = 0, reading = 0;
	char c = v[i++];
	while(COND(c) && index < max - 1){
		if (!reading && !isspace(c)){
			reading++;
		}
		if(reading){
			s[index++] = c;
		}
		c = v[i++];
	}
	s[index] = '\0';
	return;
}

/* reads a portion of a string and stores the username in s */
void readUser(char v[], char *s, int start, int max){
	int i = start, index = 0, stop = 0, reading = 0;
	char c = v[i++];
	while(COND(c) && index < max - 1 && !stop){
		if(reading == 0 && !isspace(c)){ /* the input will now be stored */
			reading++;
		}
		if(reading){
			if(isspace(c)){
				stop++;
			}
			else{
				s[index++] = c;
			}
		}
		c = v[i++];
	}
	s[index] = '\0';
	return;
}

/* returns v's index after the whitespace found after a certain number/word */
int getNextIndex(char v[], int start){
	int i = start, reading = 0;
	char c = v[i++];
	while((!reading || !isspace(c))){
		if(!COND(c)){
			return FAIL;
		}
		if(!reading && !isspace(c)){
			reading++;
		}
		c = v[i++];
	}
	return i;
}

/* used in listTasks/listAtvTasks to print the tasks' information */
void printList(task v[], int n, int func){
	int i;
	switch(func){
		case LT:
			for(i = 0; i < n; i++){
				printf("%d %s #%d %s\n", v[i].id, v[i].currAtv, v[i].pd, v[i].desc);
			}
			break;
		case LAT:
			for(i = 0; i < n; i++){
				printf("%d %d %s\n", v[i].id, v[i].st, v[i].desc);
			}
			break;
		default:
			break;
	}
	return;
}

/* checks if a given description already exists in the system */
int dupSearch(char mode, char s[]){
	int i, dup = 0;
	switch(mode){
		case TASK: /* comparing task descriptions */
			for(i = 0; i < amTasks && dup == 0; i++){
				if(!strcmp(s, taskProp[i].desc)) 
					dup++;
			}
			break;
		case USER: /* comparing user descriptions */
			for(i = 0; i < amUsers && dup == 0; i++){
				if(!strcmp(s, userProp[i].desc)) 
					dup++;
			}
			break;
		case ATV: /* comparing activity descriptions */
			for(i = 0; i < amAtvs && dup == 0; i++){
				if(!strcmp(s, atvProp[i].desc)) 
					dup++;
			}
			break; /* no need for a default case here */
	}
	return dup;
}

/* sorts a given array of tasks - 'l' command */
void shellL(int cap){
	int i, j, h;
	task tempTask;
	/* generates a h sequence - 1 4 13 40 121 ... */
	for(h = 1; h <= cap/9; h = 3*h + 1);
	for(; h > 0; h /= 3){
		for(i = h; i <= cap; i++){
			tempTask = taskProp[i];
			for(j = i; j >= h && j < cap &&
				strcmp(tempTask.desc, taskProp[j - h].desc) < 0; j -= h){
				taskProp[j] = taskProp[j - h];
			}
			taskProp[j] = tempTask;
		}
	}
	return;
}

/* sorts a given array of tasks - 'd' command */
void bubbleD(task v[], int cap){
	int max = cap - 1, changed = 1, i;
	while(changed == 1){
		changed = 0;
		for(i = 0; i < max; i++){
			if((v[i].st > v[i + 1].st) || 
			(v[i].st == v[i + 1].st && strcmp(v[i].desc, v[i + 1].desc) > 0)){
				exch(v[i], v[i + 1]);
				changed = 1;
			}
		}
		max--;
	}
	return;
}

/* aux to moveTasks, prints some of the possible error messages */
int printErrorsMove(int id, char befDesc[], char atvDesc[], int wUser, int wAtv){
	int error = 0;
	if(anyId(id, amTasks, taskProp) == FAIL){ 
		printf(NO_ID_M); /* if the ID isn't in the system */
		error ++;
	}
	else if(!strcmp(atvDesc, S_TODO) && !strcmp(befDesc, S_TODO)){
		error++; /* the program still stops if both befDesc and aDesc are S_TODO */
	}
	else if(!strcmp(atvDesc, S_TODO) && strcmp(befDesc, S_TODO)){
		printf(T_STARTED); /* if the "going to" activity is TO DO */
		error++;
	}
	else if(wUser == ZERO){
		printf(U_NOTFOUND); /* if the username is not in the system */
		error++;
	}
	else if(wAtv == ZERO){
		printf(A_NOTFOUND); /* if the activity is not in the system */
		error++;
	}
	return error;
}

/* returns the index in taskProp of the task with the given ID */
int findIndexTask(task v[], int idTemp){
	int i, res = 0, found = 0;
	for(i = 0; i < amTasks && !found; i++){
		if(v[i].id == idTemp){
			res = i;
			found++;
		}
	}
	return res;
}

/* returns the index in atvProp of the activity with the given description */
int findIndexAtv(atv v[], char description[]){
	int i, res = 0, found = 0;
	for(i = 0; i < amAtvs && !found; i++){
		if(!strcmp(description, v[i].desc)){
			res = i;
			found++;
		}
	}
	return res;
}

/* updates and print (if applicable) information regarding atvs/tasks involved in moveTasks */
void update_printMove(int beforeIndex, int afterIndex, int afterT, char beforeA[], char afterA[]){
	/* adjusts the number of tasks in the activities found above */	
	atvProp[beforeIndex].noTasks--;
	atvProp[afterIndex].noTasks++;
	strcpy(taskProp[afterT].currAtv, afterA);

	/* if the "activity being left" is TO DO, the starting time is adjusted */
	if(!strcmp(beforeA, S_TODO)){
		taskProp[afterT].st = currentTime;		
	}
	else{ /* else, the duration is adjusted */
		taskProp[afterT].duration = currentTime - taskProp[afterT].st;
	}

	taskProp[afterT].slack = taskProp[afterT].duration - taskProp[afterT].pd;

	/* only prints if the "activity being left" is not DONE and the "going to" is */
	if(!strcmp(afterA, S_DONE) && strcmp(beforeA, S_DONE)){
		printf(DURSLACK, taskProp[afterT].duration, taskProp[afterT].slack);
	}
	return;
}

/* updates and prints messages related to the addTask function */
void updateTaskVec(char desc[], int pd){
	strcpy(taskProp[amTasks].desc, desc);
	taskProp[amTasks].pd = pd;
	taskProp[amTasks].st = 0;
	taskProp[amTasks].duration = 0;
	taskProp[amTasks].id = amTasks + 1;

	strcpy(taskProp[amTasks].currAtv, S_TODO);
	atvProp[TODO].noTasks++;

	printf(T_WRITEID, taskProp[amTasks++].id);
	return;
}

/* finds (and prints) errors in addActivity */
int findErrorAddAtv(char desc[]){
	int j;
	for(j = 0; j < amAtvs; j++){
    	if (!strcmp(desc, atvProp[j].desc)){
        	printf(A_EXISTS); /* if the description is in the system */
			return FAIL;
        }
    }
    for(j = 0; j < (int) strlen(desc); j++){ 
		if(islower(desc[j])){
			printf(A_INVALID); /*if a lowercase char is found */
			return FAIL;
		}
	}

    if(amAtvs == MAX_ATV && strcmp(desc, "")){ 
    	printf(A_TOOMANY); /* if the system can't accept more atvs */
		return FAIL;
    }
	return ZERO;
}