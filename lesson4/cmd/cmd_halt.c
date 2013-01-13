//#include "functions.c" with parameters
//shutdown the computer, unfinished!!!
int cmd_halt(char *name)
{
    int i=0;
    clrscr(); //clear the screen
    new_line();
    new_line();
    new_line();
    putsn("====================================================================");
    puts(name); puts(", ");
    putsn("it's now safe to turn off the power of your computer )))");
    putsn("====================================================================");
    return 1; //return to the OS
}
