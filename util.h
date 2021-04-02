/*
 * File: util.h
 * Author: Diogo Gaspar, 99207, LEIC-A
 * Description: Program's constants, prototypes, variables, structs.
 */

/* ---------------------------------MACROS---------------------------------- */

/* max users in the system */
#define MAX_USER 50
/* max tasks in the system */
#define MAX_TASK 10000
/* max activities in the system */
#define MAX_ATV 10

/* max input length */
#define MAX_LENGTH 100000
/* max username length */
#define MAX_USERL 21
/* max task description length */
#define MAX_TASKL 51
/* max activity description length */
#define MAX_ATVL 21

/* condition to stop reading input - the user is done writing */
#define COND (c != '\0' && c != '\n' && c != EOF)
/* used in some functions to indicate that the search has failed  */
#define FAIL -1
/* beginning of the timer */
#define ZERO 0
/* used in the bubble function */
#define LT 0
/* used in the bubble function */
#define LAT 1
/* start reading from the input after considering the command character */
#define START 2

/* task-related error messages */
#define T_TOOMANY "too many tasks\n"
#define T_EXISTS "duplicate description\n"
#define T_WRITEID "task %d\n"
#define T_NOID "%d: no such task\n"
#define T_STARTED "task already started\n"
#define INV_DUR "invalid duration\n"
#define DURSLACK "duration=%d slack=%d\n"
/* user-related error messages */
#define U_TOOMANY "too many users\n"
#define U_EXISTS "user already exists\n"
#define U_NOTFOUND "no such user\n"
/* activity-related error messages */
#define A_TOOMANY "too many activities\n"
#define A_EXISTS "duplicate activity\n"
#define A_INVALID "invalid description\n"
#define A_NOTFOUND "no such activity\n"
/* time-related error messages */
#define TIME_INVALID "invalid time\n"
#define NO_ID_M "no such task\n"

/* descriptions and indexes of TO DO, IN PROGRESS and DONE, respectively */
#define TODO 0
#define S_TODO "TO DO"
#define INPROGRESS 1
#define S_INPROGRESS "IN PROGRESS"
#define DONE 2
#define S_DONE "DONE"

/* ---------------------------------STRUCTS--------------------------------- */

/* struct representing an instance of a task, including its properties */
typedef struct task{
    /* the task's predicted duration, starting time, id and actual duration */
    int pd;
    int st;
    int id;
    int duration;
    int slack;
    /* stores the task's description */
    char desc[MAX_TASKL];
    /* stores the description of the activity the task is currently in */
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
}atv;

/* -------------------------------PROTOTYPES-------------------------------- */

void addTask(char read[]);
void addUser(char read[]);
void addActivity(char read[]);
void advance(char read[]);
void listTasks(char read[]);
void listAtvTasks(char read[]);
void moveTasks(char read[]);

int anyId(int n, int size, task v[]);
int readNumber(char v[], int start);
void readUser(char v[], char *s, int start, int max);
void readTaskAtv(char v[], char *s, int start, int max);
int getNextIndex(char v[], int start);
int dupTask(char s[], task v[], int n);
int dupUser(char s[], user v[], int n);
int dupAtv(char s[], atv v[], int n);
void printTasks(task v[], int n);
void bubble(task v[], int cap, int func);
int printErrors(char atvDesc[], int wUser, int wAtv);
void update_printMove(int beforeIndex, int afterIndex, int afterT, char beforeA[], char afterA[]);

/* -------------------------------GLOBAL VARS------------------------------- */

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