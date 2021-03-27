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

void add_task(char s[], int tasks);
void list_tasks(char s[], int tasks);
void add_users(char s[], int users);
void move_task(char s[], int users, int tasks, int atv);
void list_t_atv(char s[]);
void add_atv(char s[], int atv);

/* -------------------------------------STRUCTS------------------------------------- */

/* the struct in hand represents an instance of an activity */
typedef struct activity{
	/*stores the activity's description */
	char desc[MAX_ATVL];
} atv;

/* the struct in hand represents an instance of a task */
typedef struct task{
	/* stores the task's description */
	char desc[MAX_TASKL];
	/* stores the task's predicted duration */
	int predicted;
	/* stores the task's start time */
	int startTime;
	/* stores the task's ID */
	int id;
} task;

/* the struct in hand represents an instance of a user */
typedef struct user{
	/* stores the user's description/username */
	char desc[MAX_USERL];
} user;


