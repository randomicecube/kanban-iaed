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
			case 'm':
				moveTasks(input);
				break;
			case 'd':
				listAtvTasks(input);
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
	char temp[MAX_TASKL] =  {0};
	int reading = 0;

	/* the system cannot accept more tasks */
	if(amTasks == MAX_TASK){
    	printf(T_TOOMANY);
	}
	
	else{
		/* reading the predicted duration */
		while(reading == 0 || !isspace(c)){
			if(reading == 0 && !isspace(c)){
				reading = 1;
			}
			if(isdigit((int) c)){
		 		pd = pd * 10 + (c - '0');
			}
			i++;
			c = read[i];
		}
		
		i++; /* to account for the space between <duration> and <description> */
		c = read[i];

		/* reading the task's description */
		while(COND && counter < MAX_TASKL){
			/* the temp string is used because the description might be invalid */
			temp[counter] = c;
			counter++;
			i++;
			c = read[i];
		}

		/* check if the description already exists in the system */
		for(j = 0; j < amTasks && dup == 0; j++){
			if (strcmp(temp, taskProp[j].desc) == 0){
				dup = 1;
			}
		}

		/* happens if the description exists in the system */
		if(dup == 1){
			printf(T_EXISTS);
		}

		/* if the description is valid */
		else{
			/* copying the input's information + current time into the task array */
			strcpy(taskProp[amTasks].desc, temp);
			taskProp[amTasks].pd = pd;
			taskProp[amTasks].st = currentTime;
			taskProp[amTasks].duration = 0;
			/* id always 1 ahead of amTasks */
			taskProp[amTasks].id = amTasks + 1;

			/* tasks always start in the TO DO activity */
			strcpy(taskProp[amTasks].currAtv, atvProp[TODO].desc);
			atvProp[TODO].Tasks[atvProp[TODO].noTasks] = taskProp[amTasks];
			atvProp[TODO].noTasks++;

			printf(T_WRITEID, taskProp[amTasks].id);
			amTasks++;
		}	
	}
	return;
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
    char temp[MAX_USERL] = {0};
	/* while at 0, temp doesn't store characters */
	int space = 0;

    while(COND && (space == 0 || space == 1) && counter < MAX_USERL){
		/* if the name hasn't started being written yet */
        if(space == 0){
			/* isalpha(c) being true means we can start writing the name */
			if(isalpha(c)){
				space = 1;
				temp[counter] = c;
				counter++;
			}
		}
		/* the name is being written but we found a space/tab */
		else if(space == 1 && isspace(c)){
			space = 2;
		}
		/* if the name is already being written */
		else{
			temp[counter] = c;
            counter ++;
		}

		i++;
		c = read[i];
		
    }

	/* checks if the name is already in the system */
    for(j = 0; j < amUsers && dup == 0; j++){
        if (strcmp(temp, userProp[j].desc) == 0){
            dup = 1;
        }
    }

	/* happens if the name is already in the system */
    if(dup == 1){
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
	int counter = 0, j, min = 0;
    int i = START;
    /* 0 if there's no duplicate description found, 1 if there is */
    int dup = 0;
    /* contains the currently read character from read[] */
    char c = read[i];
    /* temporary array containing an activity's description */
    char temp[MAX_ATVL] = {0};

    while(COND && counter < MAX_ATVL){
		/* checks if the activity is writen correctly - all uppercase */
		if(!isupper(c)){
			min = 1;
		}
    	temp[counter] = c;
        counter ++;
        i++;
        c = read[i];
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
	int time = 0;
	char c = read[i];
	/* if at 0, the input is considered correct; if at 1, it's not a non negative decimal */
	int wrong = 0;

	while(COND && !isspace(c)){
		/* checks if a representation of negative/float/double notation was used */
		if(c == '-' || c == '.'){
			wrong = 1;
		}
		else{
			if(isdigit((int) c)){
				time = time * 10 + (c - '0');
				i++;
				c = read[i];
			}
		}
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
	/* at 0, the system reads ID's; at 1, recognizes it as a space or tab */ 
	int space = 0;
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
		/* happens if the character being read is a digit */
		if(isdigit((int) c)){
			/* if the last character was a space/tab */
			if(space == 1){
				space = 0;
			}
			idTemp = idTemp * 10 + (c - '0');
		}
		/* if the last character was a digit and the current is a space/tab */
		else if(space == 0 && isspace(c)){
			space = 1;
			any = anyId(idTemp, amTasks, taskProp);
			/* if the ID doesn't exist in the system */
			if(any == FAIL){
				printf(T_NOID, idTemp);
			}
			else{
				taskArray[idCount] = taskProp[any];
				idCount++;
			}
			idTemp = 0;
		}

		j++;
		c = read[j];
	}

	any = anyId(idTemp, amTasks, taskProp);
	/* j > START in the condition to account for a possible initial \n or \0 or EOF */
	if(any == FAIL && j > START){
		printf(T_NOID, idTemp);
	}
	/* adds the last ID (validity checked above) */
	else if(idTemp != 0){
		taskArray[idCount] = taskProp[any];
		idCount++;
	}

	/* idCount > 0 -> list the tasks, sorted by their ID */
	if(idCount > 0){
		for(i = 0; i < idCount; i++){
			printf("%d %s #%d %s\n",taskArray[i].id, \
									taskArray[i].currAtv, \
									taskArray[i].pd, \
									taskArray[i].desc \
									);
		}
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
		for(i = 0; i < amTasks; i++){
			printf("%d %s #%d %s\n", \
					ordered[i].id, \
					ordered[i].currAtv, \
					ordered[i].pd, \
					ordered[i].desc \
					);
		}
	}
	return;
}

void listAtvTasks(char read[]){

	int i = START, j = 0, found = 0, changed = 1;
	int cap;
	char c = read[i];
	char activity[MAX_ATVL];
	task tempTask;
	atv wanted, ordered;
	
	while(COND){
		activity[j] = c;
		i++;
		j++;
		c = read[i];
	}

	printf("activity: %s\n", activity);

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
		ordered = wanted;
		/* bubble sort */
		while(changed == 1){
			changed = 0;
			for(j = 0; j < cap - 1; j++){
				if(ordered.Tasks[j].st > ordered.Tasks[j + 1].st){
					tempTask = ordered.Tasks[j];
					ordered.Tasks[j] = ordered.Tasks[j + 1];
					ordered.Tasks[j + 1] = tempTask;
					changed = 1;
				}
				else if(wanted.Tasks[j].st == ordered.Tasks[j + 1].st){
					if(strcmp(ordered.Tasks[j].desc, ordered.Tasks[j + 1].desc) > 0){
						tempTask = ordered.Tasks[j];
						ordered.Tasks[j] = ordered.Tasks[j + 1];
						ordered.Tasks[j + 1] = tempTask;
						changed = 1;
					}
				}
			}
			cap--;
		}

		for(j = 0; j < ordered.noTasks; j++){
			printf("%d %d %s\n", \
					ordered.Tasks[j].id, \
					ordered.Tasks[j].st, \
					ordered.Tasks[j].desc \
			);
		}
	}
	return;
}

void moveTasks(char read[]){
	int i = START, j, reading = 0, idTemp = 0, foundId = 0, foundUser = 0, foundAtv = 0;
	int slack = 0, erro = 0;
	char c = read[i], username[MAX_USERL], atvDesc[MAX_ATVL];
	task actualTask;
	atv actualAtv;
	while(reading == 0 || !isspace(c)){
		if(isdigit((int) c) && reading == 0){
			reading = 1;
		}
		if(reading == 1){
			idTemp = idTemp*10 + c - '0';
		}
		i++;
		c=read[i];
	}
	for(j = 0; j < amTasks && foundId == 0; j++){
		if(taskProp[j].id == idTemp){
			actualTask = taskProp[j];
			foundId = 1;
		}
	}
	if(foundId == 0){
		printf(T_NOID, idTemp);
		erro = 1;
	}

	i++;
	c = read[i];

	reading = 0, j = 0;
	while(reading == 0 || !isspace(c)){
		if(isalpha(c) && reading == 0){
			reading = 1;
		}
		if(reading == 1){
			username[j] = c;
			j++;
		}
		i++;
		c=read[i];
	}

	for(j = 0; j < amUsers && foundUser == 0; j++){
		if(strcmp(userProp[j].desc, username) == 0){
			foundUser = 1;
		}
	}

	i++;
	c = read[i];

	reading = 0, j = 0;
	while(COND){
		atvDesc[j] = c;
		j++;
		i++;
		c = read[i];
	}

	for(j = 0; j < amAtvs && foundAtv == 0; j++){
		if(strcmp(atvProp[j].desc, atvDesc) == 0){
			actualAtv = atvProp[j];
			foundAtv = 1;
		}
	}
	if(erro == 0){
		if(atvDesc == TODO){
			printf(T_STARTED);
		}
		else if(foundUser == 0){
			printf(U_NOTFOUND);
		}
		else if(foundAtv == 0){
			printf(A_NOTFOUND);
		}
		else{
			actualTask.duration = currentTime - actualTask.st;
			if(strcmp(actualAtv.desc, S_DONE) == 0){
				slack = actualTask.duration - actualTask.pd;
				printf("duration=%d slack=%d\n", actualTask.duration, slack);
			}
		}
	}
	return;
}

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