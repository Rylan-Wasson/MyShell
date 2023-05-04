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
    statFind(args, argcp);
    //pwd(args, argcp);
    //cd(args, argcp);
    //pwd(args, argcp);
    //exitProgram(args, argcp);

    //tail(args, argcp);

    //env(args, argcp);
}

static void exitProgram(char** args, int argcp)
{
  if(argcp == 1){
    exit(0);
  } else {
    int value = atoi(args[1]);
    exit(value);
  }
}

static void pwd(char** args, int argpc)
{
  long size = pathconf(".", _PC_PATH_MAX);
  char *buf = malloc((size_t) size);
  char *cwd;
  if(buf != NULL){
    cwd = getcwd(buf, (size_t)size);
  }

  printf("%s \n", cwd);
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
      printf("Size: %jd \n", stats.st_size);
      printf("Blocks: %jd \n", stats.st_blocks);
      printf("IO Block: %jd \n", stats.st_blksize);
      printf("Blocks: %jd \n", stats.st_blocks);

      printf("File type: ");
      switch (stats.st_mode & __S_IFMT) {
        case __S_IFREG:
          printf("regular file\n"); 
          break;
        case __S_IFDIR:
          printf("directory\n"); 
          break;
      }

      printf("Device: %lu\n", stats.st_dev);
      printf("Inode: %jd \n", stats.st_ino);
      printf("Links: %jd \n", stats.st_nlink);
      printf("Access: %u \n", stats.st_mode);
      printf("Uid: %u \n", stats.st_uid);
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
  int num_lines = 10;
  int max_line_length = 4095;
  int count = 0;
  char cur;
  char *buf = malloc(max_line_length);
  FILE *fp = fopen(args[1], "r");
  
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
  while(fgets(buf, max_line_length, fp) != NULL){
    printf("%s", buf);
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
        strncat(value,&(args[1][i]), 1);// this has to be strn cat for some reason?
      }
      value[size] = '\0';

      setenv(name,value,1);
    } else {
      printf("Usage: env [NAME=VALUE]");
      exit(-1);
    }
  }
  int j = 0;
  while(__environ[j] != NULL){
    printf("%s \n", __environ[j]);
    j++;
  }
    
}


