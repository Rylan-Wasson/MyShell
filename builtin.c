#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <ctype.h>
#include "builtin.h"
#include <string.h>
#include <dirent.h>
#include <time.h>

#define NUM_LINES (10)
#define MAX_LINE_LENGTH (4095);

//Prototypes
static void exitProgram(char** args, int argcp);
static void cd(char** args, int argpcp);
static void pwd(char** args, int argcp);
static void statFind(char ** args, int argcp);
static void tail(char ** args, int argcp);
static void env(char ** args, int argcp);

/* builtIn
 ** built in checks each built in command the given command, if the given command
 * matches one of the built in commands, that command is called and builtin returns 1.
 *If none of the built in commands match the wanted command, builtin returns 0;
  */
int builtIn(char** args, int argcp)
{ 
  if(strcmp(args[0], "exit") == 0){
    if(argcp > 0 && argcp < 3){
      exitProgram(args, argcp);
    }
  } else if(strcmp(args[0], "pwd") == 0){
    if(argcp == 1){
      pwd(args, argcp);
    }
  } else if(strcmp(args[0], "cd") == 0){
    if(argcp > 0 && argcp < 3){
      cd(args, argcp);
    }
  } else if(strcmp(args[0], "stat") == 0){
    if(argcp > 1){
      statFind(args, argcp);
    }
  } else if(strcmp(args[0], "tail") == 0){
    if(argcp > 1){
      tail(args, argcp);
    }
  } else if(strcmp(args[0], "env") == 0){
    if(argcp > 0 && argcp < 3){
      env(args, argcp);
    }
  } else {
    for(int i = 0; i < argcp; i++){
      free(args[i]);
    }
    free(args);
    return 0;
  }
  for(int i = 0; i < argcp; i++){
      free(args[i]);
    }
  free(args);
  return 1;
}

static void exitProgram(char** args, int argcp)
{
  if(argcp == 1){
    for(int i = 0; i < argcp; i++){
      free(args[i]);
    }
    free(args);
    exit(0);
  } else {
    for(int i = 0; i < argcp; i++){
      free(args[i]);
    }
    free(args);
    int value = atoi(args[1]);
    exit(value);
  }
}

static void pwd(char** args, int argpc)
{
  long size = pathconf(".", _PC_PATH_MAX);
  char *buf = malloc((size_t) size);
  getcwd(buf, (size_t)size);
  

  printf("%s \n", buf);
  free(buf);
}


static void cd(char** args, int argcp)
{
  char *home = "/home/";
  if(argcp == 1){
    if(chdir(home) != 0){
      perror("Error accessing home directory");
    } 
  } else {
    if(chdir(args[1]) != 0){ 
      perror("Error accessing file");
    }
  }
}

static void statFind(char** args, int argcp)
{
  struct stat stats;
  for(int i = 1; i < argcp; i++){
    if(stat(args[i], &stats) != -1){
      printf("File: %s \n", args[i]);
      printf("Size: %jd   ", stats.st_size);
      printf("Blocks: %jd   ", stats.st_blocks);
      printf("IO Block: %jd   ", stats.st_blksize);

      printf("File type: ");
      switch (stats.st_mode & __S_IFMT) {
        case __S_IFREG:
          printf("regular file\n"); 
          break;
        case __S_IFDIR:
          printf("directory\n"); 
          break;
      }

      printf("Device: %lu   ", stats.st_dev);
      printf("Inode: %jd    ", stats.st_ino);
      printf("Links: %jd    ", stats.st_nlink);
      printf("Access: %u    ", stats.st_mode);
      printf("Uid: %u   ", stats.st_uid);
      printf("Gid: %u \n", stats.st_gid);
      printf("Access: %s", ctime(&stats.st_atime));
      printf("Modify: %s", ctime(&stats.st_mtime));
      printf("Change: %s", ctime(&stats.st_ctime));
    } else {
      perror("Error fetching stats");
    }
  }
}

static void tail(char** args, int argcp)
{
  for(int i = 1; i < argcp; i++){
    int count = 0;
    char cur;
    int length = MAX_LINE_LENGTH;
    int num_lines = NUM_LINES;
    char *buf = malloc(length);
    FILE *fp = fopen(args[i], "r");
    
    if(fp == NULL){
      perror("Error opening file");
      exit(-1);
    }

    fseek(fp, 0, SEEK_END);
    // starting at end of file, decrements filepointer until num_lines lines are read
    while(count < num_lines && ftell(fp) > 0){
      fseek(fp, -1, SEEK_CUR);
      cur = fgetc(fp);
      fseek(fp, -1, SEEK_CUR);
      if(cur == '\n'){
        count++;
      }
    }
    if(ftell(fp) > 0){
      fseek(fp, 1, SEEK_CUR);
    }
    
    // print remaining lines start at new fp position
    while(fgets(buf, num_lines, fp) != NULL){
      printf("%s", buf);
    }
    printf("\n \n");
    free(fp);
    free(buf);
  }
}

static void env(char** args, int argcp)
{

  if(argcp == 2){
    int equal_location = -1;
    int size = strlen(args[1]);
    char *name = malloc(size + 1);
    char *value = malloc(size + 1); //mallocs
    
    for(int i = 0; i < size; i++){ // find location of = symbol
      if(args[1][i] == '='){
        equal_location = i;
        break;
      } 
    }

    if(equal_location > 0){
      for(int i = 0; i < equal_location; i++){ // build name string
      name[i] = args[1][i];
      }
      name[equal_location] = '\0';
    
      for(int i = equal_location+1; i < size; i++){ // build value string
        strncat(value,&(args[1][i]), 1);
      }
      value[size] = '\0';

      setenv(name,value,1);
    }
    free(name);
    free(value);
  }
  int j = 0;
  while(__environ[j] != NULL){
    printf("%s \n", __environ[j]);
    j++;
  }
}


