/**
   Author: Benjamin Burger, benjamin.burger@burgertechnologies.com
   Compile: gcc shell.c -o run

   Discription: Shell like script for bash cmds with history (!! calls most
   recently used, !N where N is a number 1-10 calls the Nth last used cmd.
   saves stats for use in the next session of this shell script. stats displays
   the used cmds and their usage.

   3/6/2018
 */

#include "headers.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>



#define MAX_LINE 80
#define MAX_ARGS 10
int CMD_COUNT = 0;
int ARGS_COUNT = 0;
char hist[MAX_ARGS][MAX_LINE];
char histholder[MAX_ARGS][MAX_LINE];
char *args[MAX_ARGS];
char one[] = "!1";
char two[] = "!2";
char three[] = "!3";
char four[] = "!4";
char five[] = "!5";
char six[] = "!6";
char seven[] = "!7";
char eight[] = "!8";
char nine[] = "!9";
char ten[] = "!10";
char input[MAX_LINE];
char inputHold[MAX_LINE];
char statTrackerArray[MAX_ARGS];
 
int should_run = 1;

//Command Strings
char runRecent[] = "!!";
char recentStr[] = "recent";
char exitStr[] = "exit";
char mostFrequentlyUsed[] = "mfu";


/**
this will house the main loop and the input collection
 */
void main()
{
  pullArray();
  while(should_run)
    { 
      printf("COMMAND-> ");
      fflush(stdout);
      
      scanf("%[^\n]%*c",&input);
      char x = input[0];

      if(memcmp(inputHold,input,sizeof(inputHold)) == 0){
	nullHandler();
	break;
      }else if(strcmp(runRecent,input)== 0)
      {
	runRecentCmd(input);
      }else if(x == '!')
      {
	singleEx(input);
      }else if(strcmp(recentStr,input) == 0)
      {
	print(hist);
      }else if(strcmp(exitStr,input) == 0)
      {
	should_run = 0;
      }else if(strcmp(mostFrequentlyUsed,input)==0){
	printFrequency();
      }else{
	runFork(input);
      }//end ifs
    }//end loop
  //  saveStats(statIntArray);
  //saveStatChars(hist);
  saveArray();
}

/**
   for error handler for entering a null as input
 */
void nullHandler()
{
  	printf("You must enter a command.\n");
	runFork("./run");
}

/**
   determines if there was a recent cmd run and if so
   exicutes the hist[0] command. 
   for use in main()
 */
void runRecentCmd(char input[MAX_LINE])
{
  	if(memcmp(histholder,hist,sizeof(histholder)) ==0){
	   printf("No commands in history.\n");
	}else{
	   printf("%s\n",hist[0]);
	   runFork(hist[0]);
	}
}


/**
   runs the cmd from hist based on the !n
   input. only goes to 10. Hard coded.
   for use in the exclimation point option of main()
*/
void singleEx(char numChar[MAX_LINE])
{
   int num = 10;
   char temp[MAX_LINE];	

   if(strcmp(one,numChar)==0){
     num = 0;
   }else if(strcmp(numChar,two)==0){
     num = 1;
   }else if(strcmp(numChar,three)==0){
     num = 2;
   }else if(strcmp(numChar,four)==0){
     num = 3;
   }else if(strcmp(numChar,five)==0){
     num = 4;
   }else if(strcmp(numChar,six)==0){
     num = 5;
   }else if(strcmp(numChar,seven)==0){
     num = 6;
   }else if(strcmp(numChar,eight)==0){
     num = 7;
   }else if(strcmp(numChar,nine)==0){
     num = 8;
   }else if(strcmp(numChar,ten)==0){
     num = 9;
   }else{
     num = 10;
   }
   
   if(num == 10 || strcmp(hist[num],"") == 0){
     printf("No such command in history.\n");
   }else{
     printf("%s\n",hist[num]);
     strcpy(temp,hist[num]);
     runFork(temp);
   }
}



/**
   this will print the last ten functions called from cmd line
*/
void print(char history[MAX_ARGS][MAX_LINE])
{
  int i = 0;
  int x = 0;
  int LEN = CMD_COUNT;
  
  for(i; i < LEN; ++i)
    {
      x = i + 1;
      printf("%i. %s\n",x,history[i]);	
    }
}


/**
   Adds input to hist array
 */
void makeCmd(char *useArr[],char input[])
{  
  strcpy(useArr[CMD_COUNT],input);
  
  ++ARGS_COUNT;
}


/**
   adds a char[] to hist array 
   made for use in runFork();
 */
void addCmdToHist(char inputArr[MAX_LINE])
{
  char temp[MAX_LINE];
  int i = 0;
  
  strcpy(temp,inputArr);
	
  for(i=CMD_COUNT ; i > 0 ; --i)
    {
      strcpy(hist[i],hist[i-1]);
    }
 
  strcpy(hist[0],temp);
  if(CMD_COUNT < MAX_ARGS)
    ++CMD_COUNT;
}


/**
   made for use in charArrToStar();
*/
static char *strShift(char *str)
{
  char *other = malloc(strlen(str) + 1);
  if(other != NULL)
    {
      strcpy(other,str);
    }
  return other;
}


/**
   creates a *char[] output from a char[]
   made for use in runFork();
*/
char *charArrToStar(char argIn[MAX_LINE])
{
  int argc = 0;
  int i = 0;
  int LEN = sizeof(&argIn) - 2;

  char *stringX = strtok(argIn," " );

  while(stringX != NULL)
    {
      args[argc++] = strShift(stringX);
      stringX = strtok(NULL," ");
    }
  args[argc] = NULL;
}


/**
   this will run a fork command and wait for the child to end and it will run the system command that is in the args array. Argsu[0] must be a terminal command with the following indexes being that command's tags.
   for use with main(); 
*/
void runFork(char argsu[])
{
  addCmdToHist(argsu);
  charArrToStar(argsu);
  
  pid_t pid;
  pid = fork();
  
  if(pid<0)
  {
    printf("Fork Failure \n");
  }else if(pid==0)
  {
    execvp(args[0],args);
    exit(127);
  }else
  {
    wait(0);
  }
}



/**
   adds a cmd to the statistic tracking array and
   increments the int value of it in statIntArray
 */
void addStatistic(char cmdArr[])
{
  int holder;
  int i;
  int LEN = sizeof(&hist);
  
  for(i = 0; i < LEN; ++i)
    {
      if(strcmp(hist[i],cmdArr) == 0)
	{
	  holder = statTrackerArray[i] + 1;
	  shiftStatIntArr();
	  statTrackerArray[0] = holder;
	}else{
	  shiftStatIntArr();
	  statTrackerArray[0] = 1;
      }
    }
}



void printFrequency()
{
  int i;
  int LEN = CMD_COUNT;

  printf("Frequencies:\n");
  for(i = 0; i < LEN ; ++i)
    {
      printf("%s : %d \n", &hist[i],statTrackerArray[i]);
    }
}

void shiftStatIntArr()
{
  
}


/**
   makes the stat history arrays bigger by 1
   used in addStatistic()
 */
void makeBigger(char cmdHist[][MAX_LINE])
{
  
}



/**
   increases the size of statIntArray by 1
   used in makeBigger()
 */
void makeIntArrBigger(int intArrToBeIncreased[])
{
  int ADDED_LEN = sizeof(&intArrToBeIncreased);
  int newArray[ADDED_LEN];
}

/**
   finds char[] in the statistic tracking array
   returns positive int if found and -1 if not found
   for use with addStatistic()
 */
int findCmd(char toFind[], char array[MAX_ARGS][MAX_LINE])
{
  int i;
  int num = -1;
  int LEN = sizeof(&array);
  for(i = 0; i < LEN; ++i)
    {
      if(strcmp(array[i],toFind))
	{
	  num = i;
	  i = LEN;
	}
    }
  return i;
}

struct record{
  int count;
  char cmd[MAX_LINE];
}entry;

void saveArray()
{
  FILE *f;

  if((f = fopen("history.bin","wb")) == NULL)
    {
      printf("ERROR OPENING FILE\n");
      exit(1);
    }

  int i = 0;
  int LEN = sizeof(&hist);
  entry.count = CMD_COUNT;
  fwrite(&entry,sizeof(struct record),1,f);

  for(i = 0; i < CMD_COUNT; i++){
    entry.count = statTrackerArray[i];
    strcpy(entry.cmd,hist[i]);
    fwrite(&entry, sizeof(struct record), 1, f);
  }
    
  fclose(f);
}


/**
   gets the data back from the history.bin and rebuilds
   the hist array.
 */
void pullArray()
{
  FILE *f;

  int i;
  int CMD_COUNT;
  if(access("history.bin",F_OK)!= -1)
    {
      f = fopen("history.bin","rb");
    }else
    {
      f = fopen("history.bin","wb");
      fclose(f);
      f = fopen("history.bin","rb");
    }

  if(f == NULL)
    {
      printf("Error opening file");
      exit(1);
    }

  fread(&entry,sizeof(struct record), 1, f);
  CMD_COUNT = entry.count;
  
  for(i = 0; i < CMD_COUNT; i++)
    {
      fread(&entry,sizeof(struct record), 1, f);
      statTrackerArray[i] = entry.count;
      addCmdToHist(entry.cmd);
    }
  
  flipHist();
  fclose(f);
}



/**
   This will reorder the hist array so that the last will be the first
   and the first will be the last
 */
void flipHist()
{
  int i;
  char tempArr[MAX_ARGS][MAX_LINE];
  for(i = 0; i < CMD_COUNT; ++i)
    {
      strcpy(tempArr[i],hist[CMD_COUNT - 1 - i]);
    }
  for(i = 0; i < CMD_COUNT; ++i)
    {
      strcpy(hist[i],tempArr[i]);
    }
}

