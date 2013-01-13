//processes each command
//returns:  1 - processable command
//          -1 - unknown command
//          0 - halt the system
int cmd_process(char *user_command, char *user_name)
{
    char command[100]; //inputed command
    puts(user_name); putsn(", enter your command.");
    puts("command: "); gets(command);
    strcpy(user_command,command);
    new_line();
    putsn("- - - - - ");
    puts("you typed: <"); puts(command); putsn(">");

    if(strcmp(command,"clrscr"))
      {
          clrscr();
          return 1;
      }

    if(strcmp(command,"halt"))
      {
          if(cmd_halt(user_name)) return 0; //halt the system
      }

    if(strcmp(command,"hello"))
    {
        new_line();
        cmd_hello(user_name);
        getchar();
    }

    else return -1; //unknown command
    //return 1; //everything is ok
}
