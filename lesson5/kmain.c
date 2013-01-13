#include "lib/video.c"
#include "lib/io.c"
#include "lib/kbd.c"
#include "lib/setjmp.h"
#include "longjmp.c"

//context
jmp_buf buf; //variable of type jmp_buf
char* vidmem = (char *) 0xb8000;

//if Mr. Kulev will know about the fact that i used global variables in my program... )))
int cursor_position=0;
int *p; //pointer to cursor_position
int line_position=0;
int *l; //pointer to line_position
int temp_pos=10*160; //position used for multitasking
//screen resolution is 800x600,
//so 80 chars per line and 60 lines in the page

//functions which where implemented in laboratory work nr.3
//output one char to the screen
void putc(char c)
{
    p=&cursor_position;
    char *vidmem=(char *) 0xb8000;
    vidmem[*p]=c;
    vidmem[(*p)+1]=0x7;
    (*p)=(*p)+2; //increment the position of the cursor
    return;
}

//length of the string
int strlen(char string[])
{
    int i=0;
    int nr_char=0; //number of chars
    while((string[i])!='\0') //terminating char
    {
        i++; //increment the position of the char in the string
        nr_char++; //increment the number of chars
    }
    return nr_char; //returns the number of chars of the string
}

//go to new line
void new_line(void)
{
    int line=0;
    l=&line_position;
    (*l)=(*l)+1; //increment the nr. of line
    (*p)=(160*(*l)); //increment the position
    return;
}

//output string to the screen
void puts(char string[])
{
    p=&cursor_position;
    int j=0;
    int length;
    length=strlen(string); //the length of the string
    for(j=0;j<length;j++)
    {
        putc(string[j]);
    }
    return;
}

//output string to the screen, and go to new line
void putsn(char string[])
{
    puts(string);
    new_line();
    return;
}

//input a string from keyboard with backspace function
void gets(char string[])
{
  int i=0;
  char ch=' ';
  p=&cursor_position;
    while(1)
    {
        if(ch!=10) //enter ascii code
        {
           ch=getchar(); //get a char from keyboard
           if((ch==8)||(ch==127)) //backspace ascii code, || - or
           {
               if(i>0)
               {
                   i=i-1;
                   string[i]='\0';
                   (*p)=(*p)-2;
                   putc(' ');
                   (*p)=(*p)-2;
               }
           }
           else
           {
               string[i]=ch; //store inputed char
               i++; //increment position
               putc(ch); //output inputed char
           }
        }
        else
        {
            (*p)=(*p)-2; //go back
            putc('\0'); //close the string
            (*p)=(*p)-1; //go back
            string[i-1]='\0'; //close the string
            break;
        }
    }
}

//clear the screen
void clrscr(void)
{
    int i=0;
    int current_position=0;
    p=&cursor_position;
    l=&line_position;
    current_position=(*p); //store cursor position
    //reset cursor position
    (*p)=0; (*l)=0;
    for(i=0;i<current_position;i++)
    {
        putc(' '); //replace char by ' ' char
    }
    //reset cursor position
    (*p)=0; (*l)=0;
    return;
}

//compare 2 strings
//returns:  0 - strings are different
//          1 - strings are equal
int strcmp(char str_1[],char str_2[])
{
    int i=0;
    int len_str_1=0; //length of the first string
    len_str_1=strlen(str_1);
    if (len_str_1!=strlen(str_2)) return 0; //different sizes of the strings
    while(i<len_str_1)
    {
        if(str_1[i]==str_2[i]) i++; //increment index position
        else return 0; //strings are different
    }
    return 1; //strings are equal
}

//copy string1 into string2
//returns:     0 - failure
//             1 - succes
int strcpy(char string1[], char string2[])
{
    int i=0;
    int str_length=0;
    str_length=strlen(string2); //the length of string2
    for(i=0;i<str_length;i++)
    {
        string1[i]=string2[i];
    }
    //compare string1 and string2
    if(strcmp(string1,string2)==1) return 1; //string are equal
    else return 0; //string are different
}

//hello with parameters
void cmd_hello(char *name)
{
    puts("Nice to meet you "); puts(name); putsn(" :)");
    return;
}

//welcome message
void cmd_welcome(char ret_name[])
{
    char name[20];
    clrscr(); //clear the screen
    putsn("================================================");
    putsn("Welcome to the alpha version of Micro-Mini OS :)");
    putsn("================================================");
    getchar();
    new_line();
    putsn("What is your name Mr. Stranger ?");
    puts("Name: ");
    gets(name); new_line(); new_line();
    if(strcpy(ret_name,name)==0) putsn("Error: invalid name(ret_name is not equal with name) ");
    puts("OK, i will call you "); putsn(name);
    cmd_hello(name); //greets the user
    return;
}

void func_2()
{
    putsn("Execute function 2");
    getchar();
	putsn("Function 2 executed");
	getchar();
	putsn("Restore the context...");
	getchar();
    longjmp(buf,1); // jumps back to where setjmp was called - making setjmp now return 1
}

//functions which where implemented in laboratory work nr.4
void func_1()
{
	putsn("Call function 2");
	getchar(); //wait for user keypress
    func_2(); //call second function
    putsn("Execute function 1");
    getchar(); //wait for user keypress
	putsn("Function 1 executed");
    getchar();
    return;
}

//code for the laboratory work nr.5-6
#define TASK_STACK_SIZE 512

int nr_task=-1;
int add_tasks=-1 ;   //number of added tasks to sched
int _sched_num_tasks=-1;  // current task

typedef enum
{
    TS_RUNNABLE = 1, TS_BLOCKED = 2, TS_ZOMBIE = 3
} task_status_t;

typedef struct
{
    jmp_buf state;
    task_status_t status;
    unsigned char stack[TASK_STACK_SIZE];
} task_t;

task_t _sched_tasks[4]; //special array of pointers to functions
void (*f)();


void yield (void) {

// switch context from current task to next
// sched_switch_context(current_task(), next_task()

int tosave=0;  // task number for whom we save the state

	if( nr_task==-1)   //no tasks previous
		{
		nr_task = 0;   //first task ( ==0 )
			if ( ! setjmp(_sched_tasks[nr_task].state) )
			{ //save its state
				switchtask(nr_task); 		//go to task 0
            }
			else {
						//no code
				 }
		 }
		else
		{

            //save state
            if(nr_task == add_tasks )
            {
                nr_task=0;  tosave = add_tasks;
            }//we reached maximum number of tasks added
            else { nr_task ++; tosave=nr_task-1;  }

            if ( ! setjmp(_sched_tasks[tosave].state) )
            {  //save state of previous function

                switchtask(nr_task); 	 //switch to next function

            }
            else
            {
                //no code
            }
     	}
    return;
}

void wait (void)
{
   int j=0;
   while ( j<9999999)
   {
       j++;
   }
}

void task0 (void)
{
  while (1)
  {
    yield();
    vidmem[temp_pos] = vidmem[temp_pos] == 'B' ? 'A' : 'B';
    //temp_pos++;
	vidmem[temp_pos+1] = 0x6;
	wait();
  }
}

void task1 (void)
{
   while (1)
   {
        yield();
        //temp_pos++;
        vidmem[temp_pos+2] = vidmem[temp_pos+2] == 'B' ? 'A' : 'B';
        //temp_pos++;
        vidmem[temp_pos+3] = 0x3;
        wait();
  }
}

void task2 (void)
{
    int temp=temp_pos-(160-2);
    while (1)
    {
        yield();
        //temp_pos++;
        vidmem[temp_pos+4] = vidmem[temp_pos+2] == 'B' ? 'A' : 'B';
        //temp_pos++;
        vidmem[temp_pos+5] = 0x9;
        wait();
    }
}

void sched_add_task (void* func_ptr) {

  //add a task to scheduler by giving a function pointer
  add_tasks++; // number of added tasks ++

  task_t *task = & _sched_tasks[++_sched_num_tasks];
  unsigned* initial_stack_ptr;

  initial_stack_ptr = (unsigned) task->stack + TASK_STACK_SIZE;

  // set stack pointer of the task context to initial_stack_ptr
  task->state[0].esp = initial_stack_ptr;
  // set instruction pointer of the task context to func_ptr
  task->state[0].eip=func_ptr;
  // set status of the task as runnable
  task->status=1;

  task->state[0].eflags = 0x200; //some magic for longjmp

}

void sched_run (void)
{

   longjmp(_sched_tasks[0].state,0);  //start with first saved task

   //or
   //f = _sched_tasks[0].state[0].eip;
   //f();

 }

 void switchtask(int a)
 {
  switch(a)
  {
    case 0:
		longjmp(_sched_tasks[0].state,0);
		break;
	case 1:
		longjmp(_sched_tasks[1].state,0);
		break;
	case 2:
		longjmp(_sched_tasks[2].state,0);
		break;
	case 3:
		longjmp(_sched_tasks[3].state,0);
	break;

  }
  return;
 }

 int main()
 {
     char name[20]; //name of the user
     cmd_welcome(name); //welcome screen
     getchar(); //wait for the user input
     new_line(); //go to new line
     sched_add_task(task0);
	 sched_add_task(task1);
	 sched_add_task(task2);
	 sched_run();
     return 0;
 }
