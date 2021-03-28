/*
 * File: util.h
 * Author: Diogo Gaspar, 99207, LEIC-A
 * Description: Program's constants, general prototypes and variables, structs.
 */

/* -------------------------------------MACROS-------------------------------------- */

/* max users in the system */
#define MAX_USER 50
/* max tasks in the system */
#define MAX_TASK 10000
/* max activities in the system */
#define MAX_ATV 10

/* max input length */
#define MAX_LENGTH 200
/* max username length */
#define MAX_USERL 20
/* max task description length */
#define MAX_TASKL 50
/* max activity description length */
#define MAX_ATVL 20

#define COND (c != '\0' && c != '\n' && c != EOF)
#define FAIL -1
/* beginning of a timer */
#define ZERO 0
/* TO DO index */
#define TODO 0
/* IN PROGRESS index */
#define INPROGRESS 1
/* DONE index */
#define DONE 2
/* start reading from the input's stream after considering the command character*/
#define START 2

/* task-related printf macros */
#define T_TOOMANY "too many tasks\n"
#define T_EXISTS "duplicate description\n"
#define T_WRITEID "task %d\n"
#define T_NOID "%d: no such task\n"
/* user-related printf macros */
#define U_TOOMANY "too many users\n"
#define U_EXISTS "user already exists\n"
/* activity-related printf macros */
#define A_TOOMANY "too many activities\n"
#define A_EXISTS "duplicate activity\n"
#define A_INVALID "invalid description\n"
/* time-related printf macros */
#define TIME_INVALID "invalid time \n"

/* -------------------------------------STRUCTS------------------------------------- */

/* struct representing an instance of a task, including its properties */
typedef struct task{
    /* variables storing the task's predicted duration, starting time and id, respectively */
    int pd;
    int st;
    int id;
    /* stores the task's description */
    char desc[MAX_TASKL];
    char currAtv[MAX_ATVL];
}task;

/* struct representing an instance of a user, including its properties */
typedef struct user{
    /* stores the user's description */
    char desc[MAX_USERL];
    /* stores the user's current task */
    task current;
}user;

/* struct representing an instance of an activity, including its properties */
typedef struct activity{
    /* stores the activity's description */
    char desc[MAX_ATVL];
    /* how many tasks are currently in the activity's scope */
    int noTasks;
}atv;

/* -----------------------------------PROTOTYPES------------------------------------ */

/* adds a task to the system - 't' command */
void addTask(char read[]);
/* adds a user to the system/lists all users by creation date - 'u' command */
void addUser(char read[]);
/* adds an activity to the system/lists all activities by creation date - 'a' command */
void addActivity(char read[]);
/* advances the system's time - 'n' command*/
void advance(char read[]);
/* lists all the tasks/a specific subset of tasks in the system - 'l' command */
void listTasks(char read[]);
/* checks if a given ID is an ID of any task in the system */
int anyId(int n, int size, task v[]);

/* -----------------------------------GLOBAL VARS----------------------------------- */

/* the program's current time */
static int currentTime = 0;

/* array with data regarding all the tasks in the system */
static task taskProp[MAX_TASK];
/* array with data regarding all the users in the system */
static user userProp[MAX_USER];
/* array with data regarding all the activities in the system */
static atv atvProp[MAX_ATV];

/* amount of tasks, users and activities in the system, respectively */
static int amTasks = 0;
static int amUsers = 0;
static int amAtvs = 0;