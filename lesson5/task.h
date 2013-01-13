#define TASK_STACK_SIZE 512



typedef enum {

TS_RUNNABLE = 1, TS_BLOCKED = 2, TS_ZOMBIE = 3

} task_status_t;



typedef struct {

jmp_buf state;

task_status_t status;

unsigned char stack[TASK_STACK_SIZE];

} task_t;
