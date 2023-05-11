#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "argparse.h"
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define FALSE (0)
#define TRUE  (1)

/*
* argCount is a helper function that takes in a String and returns the number of "words" in the string assuming that whitespace is the only possible delimeter.
*/
static int argCount(char*line)
{
  int curWord = FALSE; //currently reading a word
  int count = 0;

  /* Iterate through line */
  for(int i = 0; i < strlen(line); i++){
    if(line[i] != ' ' && curWord == FALSE){
      count++;
      curWord = TRUE;
    } else if(line[i] == ' '){
      curWord = FALSE;
    }
  }
  return count;
}



/*
*
* Argparse takes in a String and returns an array of strings from the input.
* The arguments in the String are broken up by whitespaces in the string.
* The count of how many arguments there are is saved in the argcp pointer
*/
char** argparse(char* line, int* argcp)
{
  int num_args = argCount(line);
  *argcp = num_args;
  char** args = malloc((num_args + 1) * sizeof(char*)); // free me
  int j = 0; // line index iterator

  // one iteration per arg
  for(int i = 0; i < num_args; i++){
    // push j index through leading space
    while(line[j] == ' '){
      j++;
    }
    char* arg = malloc(strlen(line) +1);
    int y = 0; // arg index iterator
    // build arg string
    while(line[j] != ' ' && line[j] != '\0'){
      arg[y] = line[j];
      j++;
      y++;
    }
    arg[y] = '\0';

    //copy arg into args
    args[i] = malloc(strlen(arg)+1);
    strcpy(args[i], arg);
    free(arg);
  }
  
  return args;
}

