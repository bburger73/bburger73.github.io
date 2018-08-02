/**
*****************Author***************************
  Name:  Benjamin Burger 
  Email: benjamin.burger@burgertechnologies.com

*****************Compile**************************
 
  gcc shell2.c -o run 

*****************Run Command**********************  

  ./run

*****************Discription***********************

  Shell like script for linux cmds with an 
  automatically saved history and usage
  statistics for use in next session of this 
  program's usage.

****************Program Commands****************** 

  !!   calls most recently used,
  !N   where N is a number 1-10 calls the Nth last 
       used cmd.
  mfu  returns the most frequently used commands
       and their given uses
  recent  returns a list of most recently ran cmds 

********************Date**************************
         
  3/6/2018

**************************************************/



#ifndef HISOTRY_H_
#define HISTORY_H_

#define MAX_LINE 80
#define MAX_ARGS 10

//save and pull helpers
void sortMFU();
void flipHist();
void shiftStatIntArr();
void addStatistic(char cmdArr[]);

//history .bin save and pull functions
void saveArray();
void pullArray();

//random helpers
char *getStatCmd();
static char *strShift(char *str);

//main() if statement functions
void nullHandler();
void printFrequency();
void runFork(char argsu[]);
void singleEx(char numChar[]);
void runRecentCmd(char input[]);
void print(char hist[][MAX_LINE]);

#endif //HISTORY_H_



//*****  Start Program  *****//

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

//*****  Variables  *****//

#define MAX_LINE 80
#define MAX_ARGS 10

//*****  control variables  *****//
int CMD_COUNT = 0;
int ARGS_COUNT = 0;
int mfuCount = 1;
int should_run = 1;

//*****  history variables  *****//
char hist[MAX_ARGS][MAX_LINE];
char histholder[MAX_ARGS][MAX_LINE];

//*****  input variables *****//
char *args[MAX_ARGS];
char input[MAX_LINE];
char inputHold[MAX_LINE];

//*****  Frequency Arrays  *****//
int statTrackerArray[MAX_LINE];
char mfuArr[MAX_LINE][MAX_LINE];

//*****  !N Commands  *****//
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

//*****  Command Strings  *****//
char runRecent[] = "!!";
char exitStr[] = "exit";
char recentStr[] = "recent";
char mostFrequentlyUsed[] = "mfu";


//*****  Program Code  *****//

/**
   Deals with the input, saving, and pulling of
   commands and determines what to do with the inputs.
   
   this is the program.
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
      if(memcmp(inputHold,input,sizeof(inputHold)) == 0)
	   {
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
	   }else if(strcmp(mostFrequentlyUsed,input)==0)
	   {
	     printFrequency();
	   }else
	   {
	     runFork(input);
	   }//end if else statements
    }//end while loop
  saveArray();
}



/**
   prints an error message
   
   used in main()
 */
void nullHandler()
{
  	printf("You must enter a command.\n");
}

/**
   determines if there was a recent cmd run and if so
   exicutes the hist[0] command. 
   
   used in main()
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
   
   used in the exclimation point option of main()
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
   prints the most recent cmds called from shell input
   maximum cmds is 10. if less than ten cmds are in hist
   this method will print all cmds.
   
   used in the "recent" option in main()
*/
void print(char history[MAX_ARGS][MAX_LINE])
{
  int i = 0;
  int x = 0;
  int LEN;

  if(CMD_COUNT < 10)
    {
      LEN = CMD_COUNT;
    }else
    {
      LEN = 10;
    }
  
  for(i; i < LEN; ++i)
    {
      x = i + 1;
      printf("%i. %s\n",x,history[i]);	
    }
}



/**
   adds a char[] command to (hist)ory array 
   
   used in runFork(); 
 */
void addCmdToHist(char inputArr[MAX_LINE])
{
  char temp[MAX_LINE];
  int i = 0;
  int newValue;
  
  strcpy(temp,inputArr);
  
  for(i=CMD_COUNT ; i > 0 ; --i)
    {
      strcpy(hist[i],hist[i-1]);
    }
  
  strcpy(hist[0],temp);
  
  ++CMD_COUNT;
  
  addStatistic(temp);
}


/**
   adds a cmd char[] to the statistic tracking array
   (mfuArr) and increases that commands index in 
   statTrackerArray.
   
   used in addCmdToHist()
 */
void addStatistic(char cmdArr[MAX_LINE])
{
  int i = 0;
  char temp[MAX_LINE];

  int LEN = mfuCount + 1;
  
  for(i = 0; i < LEN; ++i)
    {
      if(strcmp(cmdArr,mfuArr[i]) == 0)
	   {
	     statTrackerArray[i] = statTrackerArray[i] + 1;
	     break;
	   }else if(mfuCount == i)
	   {
	     strcpy(mfuArr[i - 1],cmdArr);
	     statTrackerArray[i - 1] = 1;
	     ++mfuCount;
	   }
   }
}


/**
   returns shortened by one char input(str) 
   
   used in charArrToStar();
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
   
   used in runFork();
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
   Runs argsu array as a system command.
   Argsu[0] is used as a terminal command
   with the following indexes being that 
   command's tags.
   
   Adds the cmd to (hist)ory.
   
   used in main(); 
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
   prints the frequencies for the "mfu" command

   prints at most the first 5 indexes of mfuArr
   paired up with their frequencies from 
   statTrackerArray 
   
   used in main()
 */
void printFrequency()
{
  int i;
  int LEN;
  sortMFU();
  if(mfuCount < 6)
    {
      LEN = mfuCount - 1;
    }else
    {
      LEN = 5;
    }

  printf("Frequencies: \n");
  for(i = 0; i < LEN ;++i)
    {
      printf("(%d occurrences) : %s\n", statTrackerArray[i],mfuArr[i] );
    }
}


/**
   sorts the mfuArr and statTrackerArray based on 
   the values of statTrackerArray from most 
   frequent to least frequent skipping doubled 
   Commands.
   
   used in printFrequency()
 */
void sortMFU()
{
  int i = 0;
  int j = 0;
  int LEN = CMD_COUNT;
  int tempInt;
  char tempStr[MAX_LINE];

  for(i = 0; i < LEN; ++i)
    {
      for(j = 0; j < LEN - i; ++j)
	{
	  if(statTrackerArray[j+1] > statTrackerArray[j])
	    {
	      tempInt = statTrackerArray[j];
	      statTrackerArray[j] = statTrackerArray[j+1];
	      statTrackerArray[j+1] = tempInt;
	      strcpy(tempStr,mfuArr[j]);
	      strcpy(mfuArr[j],mfuArr[j+1]);
	      strcpy(mfuArr[j+1],tempStr);
	    }
	}
    }
}



/**
   Data Structure for saving cmds and their usage
   
   used in saveArray() and pullArray()
 */
struct record{
  int count;
  char cmd[MAX_LINE];
}entry;



/**
   saves the history array and the 
   statTrackerArray to history.bin
   
   used in main();
 */
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
  
  for(i = 0; i < CMD_COUNT; ++i)
  {
    entry.count = statTrackerArray[i];
    strcpy(entry.cmd,hist[i]);
    fwrite(&entry, sizeof(struct record), 1, f);
  }
    
  fclose(f);
}


/**
   rebuilds (hist)ory array and statTrackerArray from
   history.bin
   
   used in main();
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
   Reorders the hist array so that the last will be the first
   and the first will be the last. 
   
   used in pullArray();
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


