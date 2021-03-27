/*
 * File: util.h
 * Author: Diogo Gaspar, 99207, LEIC-A
 * Description: Program's constants, general prototypes, typedef's
 */


/* ------------------------------------CONSTANTS------------------------------------ */

/* max users in the system */
#define MAX_USER 50
/* max tasks in the system */
#define MAX_TASK 10000
/* max activities in the system */
#define MAX_ATV 10
/* max input length */
#define MAX_LENGTH 200

/* all the lengths below include the '\0' character */

/* max username length */
#define MAX_USERL 20
/* max task description length */
#define MAX_TASKL 50
/* max activity description length */
#define MAX_ATVL 20


/* beginning of a timer */
#define ZERO 0
/* start reading from the input's stream */
#define START 2

/* -----------------------------------PROTOTYPES------------------------------------ */

void addTask(char read[]);
void addUser(char read[]);
void addActivity(char read[]);
void advance(char read[]);

/* -----------------------------------GLOBAL VARS----------------------------------- */

/* stores the program's state - at 0 the program keeps running, at 1 it stops */
static int state = 0;
/* the program's current time */
static int currentTime = 0;
/* stores the amounts of tasks, users and activities, respectively */
static int amounts[3];
/* stores the tasks' predicted duration, starting time and id, respectively */
static int pTask[MAX_TASK][3];
/*stores all the tasks' descriptions */
static char dTask[MAX_TASK][MAX_TASKL];
/*stores the activity the tasks are currently in */
/*static char aTask[MAX_TASK][MAX_ATVL];*/
/* stores the users' descriptions */
static char user[MAX_USER][MAX_USERL];
/* stores the activities' descriptions */
static char atv[MAX_ATV][MAX_ATVL];
