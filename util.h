/* MAXIMUM AMOUNTS */

/* max users in the system */
#define MAX_USER 50
/* max tasks in the system */
#define MAX_TASK 10000
/* max activities in the system */
#define MAX_ATV 10

/* all the lengths below include the '\0' character */

/* max username length */
#define MAX_USERL 20
/* max task description length */
#define MAX_TASKL 50
/* max activity description length */
#define MAX_ATVL 20


/* beginning of a timer */
#define ZERO 0


/* main functions' prototypes */

void add_task(char s[], int tasks);
void list_tasks(char s[], int tasks);
void add_users(char s[], int users);
void move_task(char s[], int users, int tasks, int atv);
void list_t_atv(char s[]);
void add_atv(char s[], int atv;

