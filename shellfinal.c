#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <ctype.h>
#include <fcntl.h>


#define MAX_WORD 10 /*max number of words in line is 10*/
#define MAX_CHAR 100 /*max number of chars in line is 100*/

extern char **environ;

/*
 Start
 */
 
//split the command by delim.  ls > ./file
char ** split(char * line, char * delim) {
  int size = 100;
 
  char ** commands = malloc(size * sizeof(char * ));
  char * command = NULL;
 
  int i = 0;
  command = strtok(line, delim);
 
  while (command != NULL) {
    commands[i] = command;
    i++;
    command = strtok(NULL, delim);
  }
  commands[i] = NULL;
  return commands;
}
 
void remove_spaces(char * s) {
  const char * d = s;
  do {
    while ( * d == ' ') {
      ++d;
    }
  } while ( * s++ = * d++);
}
/*
 End
 */

/*Function "process_line" :take the line and split it in args[]*/
int process_line(char * args[], char * line[]) {
  int i = 0;
  args[i] = strtok(line, " "); /*take line and replace each " " with \0*/

  if (args[i] == NULL) /*nothing in line*/ {
    return 1;
  }

  while (args[i] != NULL) {
     args[++i] = strtok(NULL, " ");
  }

  

  return 1;

}

/*Function "remove_endOfLine" : at the end of line we press enter so the last char \n here we remove it*/
void remove_endOfLine(char line[]) {
  int i = 0;
  while (line[i] != '\n')
    i++;

  line[i] = '\0';
}

/*Function "list_environ" : execute the command "environ"  */
int list_environ(void) {
    char ** env = environ;
    while ( * env) fprintf(stdout, "%s\n", * env++);
    return 0;
}
 
/*Function "lsh_cd" : to change the directory*/
void lsh_cd(char ** args) {
  if (args[1] == NULL) {
    fprintf(stderr, "simple shell: expected argument to \"cd\"\n");
  } 
  else {
        if (chdir(args[1]) != 0) {
            perror("simple shell");
        } else {
            printf("the directory has been changed successfully \n");
        }

  }

}

/*read from console in line[]*/
//int read_line(char * line, char * args[]) {
int read_line(char * line[], char * args[], char * outputfile) {

  char * ret = fgets(line, MAX_CHAR, stdin); /*stdin : read from console*/
  remove_endOfLine(line); /*remove \n*/
  char ** commands = split(line, ">");
  
  ret =process_line(args, line);
 
 if (commands[1] != NULL) {

    strcpy(outputfile, commands[1]);
    remove_spaces(outputfile);
  }
 
  if (strcmp(line, "exit") == 0 || strcmp(line, "quit") == 0) { //"quit" or "exet" command >> to exit shell
    return 0;
  }

  if (strcmp(line, "clr") == 0) { //"clr" command >>> for clear
    system("clear");
    return -1;
  }

  if (strncmp(line, "cd", 2) == 0) { //"cd <directory>" command >>> change directory
     lsh_cd(args);
    return -1;
  }
 
  if (strcmp(args[0], "myshell")==0) {
	char * name ;
	strcpy(name, args[1]);
	//printf("%s" , args[1]);
	FILE * bat = popen(name, "w");
	//system("./file.bat");
	//fprintf(bat,"from bat");
        pclose(bat);
	return -1;
  }
  
   if (!strcmp(line, "environ")) { // "environ" command 
        if (list_environ() == 0) {
           return -1;
        } else {
           perror("Simple shell");
        }
  }

   if (!strcmp(args[0], "pause")){  // "pause" command >> to stop process till press enter
      getpass("Paused\npress <Enter> key to continue");
      return -1;
  }

 
    
   if (strcmp(line, "help") == 0) { //"help" command >> for help
    printf("\nHelp for our <<Simple Shell>>\n i Type a command to run it.\n ii The builtin commands are :\n   1.  ls [optional]       : List files in the current directory or <directory> .\n   2.  dir [optional]      : List files in the current directory or <directory> .\n   3.  myshell <directory> : take command from files.\n   4.  echo <comment>      : Echo the <comment> to standard output.\n   5.  cd <directory>      : change current directory to <directory> . \n   6.  clr                 : clears your screen if this is possible.\n   7.  cat                 : concatenate files and print on the standard output.\n   8.  pwd                 : print name of current/working directory.\n   9.  pause               : wait for signal.\n   10. environ             : List all environment strings. \n   11. help                : run this command , and provide info about shell.\n   12. quit                : Leave Simple Shell.\n   13. exit                : Leave Simple Shell.\n");
    return -1;
  }

      return 1;/*pause - 
*/
}


int main() {

  char * args[MAX_WORD];
  char * line  = (char*) malloc((MAX_CHAR+1)*sizeof(char)); /*+1 for '\0' character */
  char * outputfile[MAX_CHAR];
  int finish = 0;
  char * user = getenv("USER");
  char hostname[1024];
  char * pwd = getenv("PWD");
  char lineCopy[MAX_CHAR] = {0};
  char * command = NULL;
  char ** ret;

  // Print out a welcome message
  printf("Welcome to Simple Shell\n");
  
  //print out hostname
  gethostname(hostname, 1024);

  
  while (!finish) {
       
        char cwd[1024]; 
        int size = 100;
        outputfile[0] = '\0';

         // current work directory
        getcwd(cwd, sizeof(cwd));

        // Print out prompt
        printf("\n[");
        printf(user);
        printf("@");
        printf(hostname);
        printf("] ");
        printf(" %s $ ", cwd);
        
        int check_line = read_line(line, args, outputfile);
        
      
        
        if (check_line == 0) {
            // If NULL, leave
            finish = 1;
            printf("\nLeaving ..\n");
            exit(0);
        } 
        else if (check_line == -1){
            continue;
        }
        else {
            // If there is something there...
            if (strcmp(line, "") == 0) {
                fprintf(stderr, "shell: Expected command\n");
            }
             else if (strcmp(line, "quit") == 0) {
                exit(0);
            } 
            else {
                // Run the command
                pid_t pid;
                int err;
                pid = fork();

                if (pid == -1) {
                perror("simple shell");

                } 
                else if (pid == 0) {
		        if (outputfile[0] != '\0') {
			    int fd = open(outputfile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
			    dup2(fd, 1);
			    dup2(fd, 2);
			    close(fd);
		        }
		        execvp(args[0], args);

                } 
                else {
                waitpid(pid, &err, 0);
                 
                }
            }
        }
  }
  return 0;
}



