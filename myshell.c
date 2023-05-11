/* CS 347 -- Mini Shell!
 * Original author Phil Nelson 2000
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include "argparse.h"
#include "builtin.h"


/* PROTOTYPES */

void processline (char *line);
ssize_t getinput(char** line, size_t* size);

/* main
 * This function is the main entry point to the program.  This is essentially
 * the primary read-eval-print loop of the command interpreter.
 */

int main () {

 //write your code
 //use getinput and processline as appropriate
  
  char* line = "cd ..";
  processline(line);
  
  line = "pwd";
  processline(line);

  line = "cd MyShell";
  processline(line);

  line = "pwd";
  processline(line);

  line = " ";
  processline(line);

  //args[0] = "pwd";
  //builtIn(args, 1);
  return EXIT_SUCCESS;
}


/* getinput
* line     A pointer to a char* that points at a buffer of size *size or NULL.
* size     The size of the buffer *line or 0 if *line is NULL.
* returns  The length of the string stored in *line.
*
* This function prompts the user for input, e.g., %myshell%.  If the input fits in the buffer
* pointed to by *line, the input is placed in *line.  However, if there is not
* enough room in *line, *line is freed and a new buffer of adequate space is
* allocated.  The number of bytes allocated is stored in *size. 
* Hint: There is a standard i/o function that can make getinput easier than it sounds.
*/
ssize_t getinput(char** line, size_t* size) {
  printf("%s ", "%myshell%");
  ssize_t len = getline(line, size, stdin);
  
  return len;
}


/* processline
 * The parameter line is interpreted as a command name.  This function creates a
 * new process that executes that command.
 * Note the three cases of the switch: fork failed, fork succeeded and this is
 * the child, fork succeeded and this is the parent (see fork(2)).
 * processline only forks when the line is not empty, and the line is not trying to run a built in command
 */
void processline (char *line)
{
  if (strlen(line) > 0){
    pid_t cpid;
    int   status;
    int argCount;
    char** arguments = argparse(line, &argCount);
    
    /*check whether arguments are builtin commands
    *if not builtin, fork to execute the command.
    */
      //write your code

    if(builtIn(arguments, argCount) == 0){
      cpid = fork();
      if(cpid == -1){
        perror("Error forking");
      } else if(cpid == 0){
        execl("/bin/sh", "sh", "-c", line, NULL);
      } else if(cpid > 0){
        wait(&status);
      }
    }
    
  }
    
  
}

