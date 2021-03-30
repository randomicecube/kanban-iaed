/*
 * File: util.h
 * Author: Diogo Gaspar, 99207, LEIC-A
 * Description: Program's constants, prototypes, variables, structs.
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
#define MAX_USERL 21
/* max task description length */
#define MAX_TASKL 51
/* max activity description length */
#define MAX_ATVL 21

/* condition to stop reading input - the user is done writing */
#define COND (c != '\0' && c != '\n' && c != EOF)
/* used in some functions to indicate that the search has failed - no matches */
#define FAIL -1
/* beginning of the timer */
#define ZERO 0
/* start reading from the input's stream after considering the command character*/
#define START 2

/* task-related printf macros */
#define T_TOOMANY "too many tasks\n"
#define T_EXISTS "duplicate description\n"
#define T_WRITEID "task %d\n"
#define T_NOID "%d: no such task\n"
#define T_STARTED "task already started\n"
/* user-related printf macros */
#define U_TOOMANY "too many users\n"
#define U_EXISTS "user already exists\n"
#define U_NOTFOUND "no such user\n"
/* activity-related printf macros */
#define A_TOOMANY "too many activities\n"
#define A_EXISTS "duplicate activity\n"
#define A_INVALID "invalid description\n"
#define A_NOTFOUND "no such activity\n"
/* time-related printf macros */
#define TIME_INVALID "invalid time \n"

/* descriptions and indexes of TO DO, IN PROGRESS and DONE */
#define TODO 0
#define S_TODO "TO DO"
#define INPROGRESS 1
#define S_INPROGRESS "IN PROGRESS"
#define DONE 2
#define S_DONE "DONE"


/* -------------------------------------STRUCTS------------------------------------- */

/* struct representing an instance of a task, including its properties */
typedef struct task{
    /* variables storing the task's predicted duration, starting time, id and actual duration */
    int pd;
    int st;
    int id;
    int duration;
    /* stores the task's description */
    char desc[MAX_TASKL];
    char currAtv[MAX_ATVL];
}task;

/* struct representing an instance of a user, including its properties */
typedef struct user{
    /* stores the user's description */
    char desc[MAX_USERL];
}user;

/* struct representing an instance of an activity, including its properties */
typedef struct activity{
    /* stores the activity's description */
    char desc[MAX_ATVL];
    /* how many tasks are currently in the activity's scope */
    int noTasks;
    /* descriptions of the tasks currently in the activity's scope */
    task Tasks[MAX_TASK];
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
/* lists all the tasks currently in a given activity */
void listAtvTasks(char read[]);
/* checks if a given ID is an ID of any task in the system */
int anyId(int n, int size, task v[]);
void moveTasks(char read[]);

/* -----------------------------------GLOBAL VARS----------------------------------- */

/* the program's current time */
int currentTime = 0;

/* array with data regarding all the tasks in the system */
task taskProp[MAX_TASK];
/* array with data regarding all the users in the system */
user userProp[MAX_USER];
/* array with data regarding all the activities in the system */
atv atvProp[MAX_ATV];

/* amount of tasks, users and activities in the system, respectively */
int amTasks = 0;
int amUsers = 0;
int amAtvs = 0;