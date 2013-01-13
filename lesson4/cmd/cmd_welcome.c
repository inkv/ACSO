//#include "cmd_hello.c"
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
