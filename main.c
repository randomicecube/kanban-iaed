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

	/* stores the program's state - at 0 the program keeps running, at 1 it stops */
	int state = 0;

	/* starting the 3 initial activities */
	strcpy(atvProp[TODO].desc, S_TODO);
	atvProp[TODO].noTasks = 0;
	strcpy(atvProp[INPROGRESS].desc, S_INPROGRESS);
	atvProp[INPROGRESS].noTasks = 0;
	strcpy(atvProp[DONE].desc, S_DONE);
	atvProp[DONE].noTasks = 0;
	/* updating the activity count */
	amAtvs += 3;

	do{
		fgets(input, MAX_LENGTH, stdin);
		/* input[0] was used since the command character is always the first character */
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
		/* clear the input array for the next stream of input */
		strcpy(input, "");
	}while(!state);

	return 0;
}

/* adds a task to the system - 't' command */
void addTask(char read[]){

	/* index of the character being read */
	int index;
	/* predicted duration */
	int pd = 0;
	/* temporary array containing a task's description */
	char temp[MAX_TASKL] = {0};

	/* the system cannot accept more tasks */
	if(amTasks == MAX_TASK){
    	printf(T_TOOMANY);
	}
	
	else{
		pd = readNumber(read, START);
		index = getNextIndex(read, START);

		/* reading the task's description */
		strcpy(temp, readTaskAtv(read, index, MAX_TASKL));

		/* happens if the description exists in the system */
		if(dupSearch(DUP_TASK, temp, amTasks) == 1){
			printf(T_EXISTS);
		}

		/* if the description is valid */
		else{
			/* copying the input's information + current time into the task array */
			strcpy(taskProp[amTasks].desc, temp);
			taskProp[amTasks].pd = pd;
			taskProp[amTasks].st = 0;
			taskProp[amTasks].duration = 0;
			/* id always 1 ahead of amTasks */
			taskProp[amTasks].id = amTasks + 1;

			/* tasks always start in the TO DO activity */
			strcpy(taskProp[amTasks].currAtv, atvProp[TODO].desc);
			atvProp[TODO].noTasks++;

			printf(T_WRITEID, taskProp[amTasks].id);
			amTasks++;
		}	
	}
	return;
}

/* adds a user to the system/lists all users by creation date - 'u' command */
void addUser(char read[]){
	int j;
    /* temporary array containing a user's description */
    char temp[MAX_USERL] = {0};
	/* while at 0, temp doesn't store characters */
	
	strcpy(temp, readUser(read, START, MAX_USERL));
    if(dupSearch(DUP_USER, temp, amUsers) == 1){
        printf(U_EXISTS);
    }
	/* happens if the system can't accept more users */
	else if(amUsers == MAX_USER){
		printf(U_TOOMANY);
	}
	/* happens if the system is adding a user */
    else if(strcmp(temp, "") != 0){
    	strcpy(userProp[amUsers].desc, temp);
        amUsers++;
 	}
	 /* happens if the request is to print all usernames */
	else{
		for(j = 0; j < amUsers; j++){
			printf("%s\n", userProp[j].desc);
		}
	}
	return;
}


/* adds an activity to the system/lists all activities by creation date - 'a' command */
void addActivity(char read[]){
	int j, min = 0;
    /* 0 if there's no duplicate description found, 1 if there is */
    int dup = 0;
    /* temporary array containing an activity's description */
    char temp[MAX_ATVL] = {0};

	strcpy(temp, readTaskAtv(read, START, MAX_ATVL));

    for(j = 0; j < (int) strlen(temp); j++){
		if(islower(temp[j])){
			min = 1;
		}
	}

	/* checks if the description is already in the system */
    for(j = 0; j < amAtvs && dup == 0; j++){
    	if (strcmp(temp, atvProp[j].desc) == 0){
        	dup = 1;
        }
    }

	/* happens if the description is already in the system */
    if(dup == 1){
    	printf(A_EXISTS);
    }
	/* happens if the description was written wrongly - including lowercase characters */
	else if(min == 1){
		printf(A_INVALID);
	}
	/* happens if the system can't accept more activities */
    else if(amAtvs == MAX_ATV && strcmp(temp, "") != 0){
    	printf(A_TOOMANY);
    }
	/* happens if the request is to add an activity */
    else if(strcmp(temp, "") != 0){
    	strcpy(atvProp[amAtvs].desc, temp);
        amAtvs++;
    }
	/* happens if the request is to list all activities */
    else{
    	for(j = 0; j < amAtvs; j++){
        	printf("%s\n", atvProp[j].desc);
        }
    }
	return;
}

/* advances the system's time - 'n' command*/
void advance(char read[]){
	
	int i = START;
	int time = 0, reading = 0;
	char c = read[i];
	/* if at 0, the input is considered correct; if at 1, it's not a non negative decimal */
	int wrong = 0;

	while(COND && (!isspace(c) || reading == 0)){
		/* checks if a representation of negative/float/double notation was used */
		if(c == '-' || c == '.'){
			wrong = 1;
		}
		else{
			if(isdigit((int) c)){
				if(reading == 0){
					reading = 1;
				}
				time = time * 10 + (c - '0');
			}
		}
		i++;
		c = read[i];
	}

	/* happens if the time isn't invalid */
	if(wrong != 1){
		if(time != 0){
			currentTime += time;
		}

		printf("%d\n", currentTime);
	}
	/* happens if the time is invalid */
	else{
		printf(TIME_INVALID);
	}
	return;
}

/* lists all the tasks/a specific subset of tasks in the system - 'l' command */
void listTasks(char read[]){
	int i, j = START;
	/* used with the anyID function; at FAIL, no matching id; otherwise, contains the id */
	int any;
	/* contains a temporary value of an ID  */
	int idTemp = 0;
	/* how many ID's have been counted from the input stream */
	int idCount = 0;
	/* used in the sorting algorithm - 1 if there were changes made, 0 if not */ 
	int changed = 1;
	/* used in the sorting algorihtm, how many "passes" the loop has to do */
	int cap = amTasks;
	/* contain an ordered subset of tasks and a general subset of tasks, respectively */
	task ordered[MAX_TASK] = {0}, taskArray[MAX_TASK] = {0};
	task tempTask;
	char c = read[j];

	while(COND){
		idTemp = readNumber(read, j);
		j = getNextIndex(read, j);
		if(idTemp > 0){
			any = anyId(idTemp, amTasks, taskProp);
			if(any == FAIL){
				printf(T_NOID, idTemp);
			}
			else{
				taskArray[idCount] = taskProp[any];
				idCount++;
			}
		}
		c = read[j];
	}

	/* idCount > 0 -> list the tasks, sorted by their ID */
	if(idCount > 0){
		printTasks(taskArray, idCount);
	}

	/* idCount == 0 -> list the tasks, sorted alphabetically by their descriptions */
	else{
		memcpy(ordered, taskProp, sizeof(taskProp));
		/* bubble sort */
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
		printTasks(ordered, amTasks);
	}
	return;
}

void listAtvTasks(char read[]){

	int i, j = 0, found = 0, index = 0, changed = 1;
	int cap;
	char activity[MAX_ATVL];
	task tempTask, ordered[MAX_TASK];
	atv wanted;
	
	strcpy(activity, readTaskAtv(read, START, MAX_ATVL));

	for(j = 0; j < amAtvs && found == 0; j++){
		if(!strcmp(atvProp[j].desc, activity)){
			found = 1;
			wanted = atvProp[j];
		}
	}

	if(found == 0){
		printf(A_NOTFOUND);
	}

	else{
		cap = wanted.noTasks;
		for(i = 0; i < amTasks; i++){
			if(strcmp(taskProp[i].currAtv, wanted.desc) == 0){
				ordered[index] = taskProp[i];
				index++;
			}
		}
		/* bubble sort */
		while(changed == 1){
			changed = 0;
			for(j = 0; j < cap - 1; j++){
				if(ordered[j].st > ordered[j + 1].st){
					tempTask = ordered[j];
					ordered[j] = ordered[j + 1];
					ordered[j + 1] = tempTask;
					changed = 1;
				}
				else if(ordered[j].st == ordered[j + 1].st){
					if(strcmp(ordered[j].desc, ordered[j + 1].desc) > 0){
						tempTask = ordered[j];
						ordered[j] = ordered[j + 1];
						ordered[j + 1] = tempTask;
						changed = 1;
					}
				}
			}
			cap--;
		}

		for(j = 0; j < wanted.noTasks; j++){
			printf("%d %d %s\n", \
					ordered[j].id, \
					ordered[j].st, \
					ordered[j].desc \
			);
		}
	}
	return;
}

void moveTasks(char read[]){
	int i = START, idTemp = 0;
	int wrongUser = 0, wrongAtv = 0;
	char temp[MAX_ATVL], username[MAX_USERL] = {0}, atvDesc[MAX_ATVL] = {0};
	task wantedTask;
	atv beforeAtv, wantedAtv;
	
	idTemp = readNumber(read, i);
	i = getNextIndex(read, i);
	if(anyId(idTemp, amTasks, taskProp) == FAIL){
		printf(T_NOID, idTemp);
	}
	
	strcpy(username, readUser(read, i, MAX_USERL));
	i = getNextIndex(read, i);
	if(dupSearch(DUP_USER, username, amUsers) == ZERO){
		wrongUser = 1;
	}

	strcpy(atvDesc, readTaskAtv(read, i, MAX_ATVL));
	if(dupSearch(DUP_ATV, atvDesc, amAtvs) == ZERO){
		wrongAtv = 1;
	}

	if(strcmp(atvDesc,S_TODO) == 0){
		printf(T_STARTED);
	}
	else if(wrongUser == 1){
		printf(U_NOTFOUND);
	}
	else if(wrongAtv == 1){
		printf(A_NOTFOUND);
	}
	else{
		for(i = 0; i < amTasks; i++){
			if(taskProp[i].id == idTemp){
				wantedTask = taskProp[i];
			}
		}
		strcpy(temp, wantedTask.currAtv);
		for(i = 0; i < amAtvs; i++){
			if(strcmp(atvDesc, atvProp[i].desc) == 0){
				wantedAtv = atvProp[i];
			}
			else if(strcmp(temp, atvProp[i].desc) == 0){
				beforeAtv = atvProp[i];
			}
		}
		
		beforeAtv.noTasks--;
		wantedAtv.noTasks++;

		strcpy(wantedTask.currAtv, wantedAtv.desc);

		if(strcmp(temp, S_TODO) == 0){
			wantedTask.duration = 0;
			if(strcmp(atvDesc, S_DONE) != 0){
				wantedTask.st = currentTime;
			}
		}
		else{
			wantedTask.duration = currentTime - wantedTask.st;
		}
		wantedTask.slack = wantedTask.st - wantedTask.pd;

		if(strcmp(atvDesc, S_DONE) == 0){
			printf("duration=%d slack=%d\n", wantedTask.duration, wantedTask.slack);
		}
	}
	return;
}




/* aux functions*/




/* returns -1 if the ID isn't in the system or its index in the tasks array if it is */
int anyId(int n, int size, task v[]){
	int i, res = FAIL;
	for(i = 0; i < size; i++){
		if(v[i].id == n){
			res = i;
		}
	}
	return res;
}


/* aux function, works with pd and id */
int readNumber(char v[], int start){
	int i = start, reading = 0, res = -1; /* if -1, it never read anything */
	char c = v[i];
	while(reading == 0 || !isspace(c)){
		if(reading == 0 && !isspace(c)){
			res = 0;
			reading = 1;
		}
		if(isdigit((int) c)){
		 	res = res * 10 + (c - '0');
		}
		i++;
		c = v[i];
	}
	return res;
}

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

const char* readTaskAtv(char v[], int start, int max){ 
	int i = start, index = 0;
	char *res = malloc(MAX_TASKL), temp[MAX_TASKL] = {0}, c = v[i];
	while(COND && index < max){
		temp[index] = c;
		index++;
		i++;
		c = v[i];
	}
	strcpy(res, temp);
	return res;
}

const char* readUser(char v[], int start, int max){
	int i = start, index = 0, space = 0;
	char *res = malloc(MAX_USERL), temp[MAX_USERL] = {0}, c = v[i];
	while(COND && (space == 0 || space == 1) && index < max){
		/* if the name hasn't started being written yet */
        if(space == 0){
			/* isalpha(c) being true means we can start writing the name */
			if(isalpha(c)){
				space = 1;
				temp[index] = c;
				index++;
			}
		}
		/* the name is being written but we found a space/tab */
		else if(space == 1 && isspace(c)){
			space = 2;
		}
		/* if the name is already being written */
		else{
			temp[index] = c;
            index ++;
		}

		i++;
		c = v[i];
	}
	strcpy(res, temp);
	return res;
}

void printTasks(task v[], int n){
	int i;
	for(i = 0; i < n; i++){
		printf("%d %s #%d %s\n", v[i].id, v[i].currAtv, v[i].pd, v[i].desc);
	}
	return;
}

int dupSearch(char v, char s[], int n){
	
	int i, dup = 0;

	switch(v){
		case DUP_TASK:
			for(i = 0; i < n && dup == 0; i++){
				if(strcmp(s, taskProp[i].desc) == 0){
					dup = 1;
				}
			}
			break;
		case DUP_USER:
			for(i = 0; i < n && dup == 0; i++){
				if(strcmp(s, userProp[i].desc) == 0){
					dup = 1;
				}
			}
			break;
		case DUP_ATV:
			for(i = 0; i < n && dup == 0; i++){
				if(strcmp(s, atvProp[i].desc) == 0){
					dup = 1;
				}
			}
			break;
		default:
			break;
	}
	return dup;
}