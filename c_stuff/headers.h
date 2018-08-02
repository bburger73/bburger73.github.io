
#ifndef HISOTRY_H_
#define HISTORY_H_


#define MAX_LINE 80
#define MAX_ARGS 10

void printFrequency();

void shiftStatIntArr();

void flipHist();
/**
   saves an array to history.txt
 */
void saveArray();

/**
   pulls array of ints from histstat.txt 
 */
void pullArray();
/**
   pulls array of chars from histcmd.txt
 */
char *getStatCmd();

/**
   pulls the 10 last used cmds from history.txt
 */
char *getArray();

void singleEx(char numChar[]);

void runRecentCmd(char input[]);

void print(char hist[][MAX_LINE]);

void makeCmd(char *useArr[],char input[]);

void addCmd(char inputArr[]);

static char *strShift(char *str);

char *convert(char argIn[]);

void runFork(char argsu[]);

void nullHandler();

void saveCmd(char cmdArr[]);

void addStatistic(char cmdArr[]);

int findCmd(char toFind[], char array[][MAX_LINE]);


#endif //HISTORY_H_
