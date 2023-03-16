#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define NUM_OF_FILES 10
#define NAME_INDEX 0
#define MULTITHREAD_INDEX 1
#define PRINT_INDEX 2

// Global variables
float globalMinTemp, globalMaxTemp;
long globalLinesRead = 0;
int global_filesRead = 0;;
int multithreading = 0;
int statsPrint = 0;

// initialize mutexes
pthread_mutex_t min_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t max_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lines_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t files_mutex = PTHREAD_MUTEX_INITIALIZER;

char *cityNames[NUM_OF_FILES] = 
{
    // alphabetical order
    "Charlottetown", 
    "Edmonton",
    "Halifax",
    "Montreal",
    "Ottawa",
    "Quebec",
    "Toronto",
    "Vancouver",
    "Victoria",
    "Winnipeg"
};

// function prototypes

void PrintSeparator();

void PrintCityData(char *city, float minTemp, float maxTemp, float averageTemp, int linesProcessed);

char *Concat(char *string1, char *string2); 

void *ProcessFile(void *cityIndex);

// function bodies

void PrintSeparator(int count)
{
    // print separator
    for(int i = 1; i <= count; i++)
    {
        printf("=");
    }
}

// Print data gathered and calculated for each city file.
void PrintCityData(char *city, float minTemp, float maxTemp, float averageTemp, int linesProcessed)
{
    PrintSeparator(40);
    
    // Here, "total values" means the number of valid lines processed. Not individual numbers.
        printf("\nData for: %s city\n%s's highest temperature: %2.3f\n%s's lowest temperature: %2.3f\n%s's average temperature: %2.3f\nTotal values processed for %s is: %d\n",city,city,maxTemp,city,minTemp,city,averageTemp,city,linesProcessed);

    }

// concatenate string1 and string2
// resultant string must be free()'d when no longer needed!
char *Concat(char *string1, char *string2)
{
    char *newString = malloc(strlen(string1) + strlen(string2) + 1);
    strcpy(newString, string1);
    strcat(newString, string2);
    return newString;
}


// Open and read file, and calculate required values.
// Can run on a thread.
void *ProcessFile(void *cityIndex)
{
    // cityIndex is an address (a void pointer), but we need an int. 
    // First cast cityIndex to an int pointer, then access it's value (an address).
    int index = *(int *)cityIndex;

    // opening file
    FILE *fd;
    char *fileDirectory = "data_files/";     // location of files
    char *fileType = ".dat";     // file suffix
    char *city = cityNames[index];

    // reading file
    char *line = NULL;          // line we read from the file
    size_t lineLength = 0;
    ssize_t getlineResult;
    int linesRead = 0;          // record how many lines of the file we have gotten
    int currentLine = 1;        // record which line of the file we are getting
    int isMaxColumn = 1;        // indicates if the current token is from the max column (first column)
   
    // for strtok_r
    const char *delim = "\t";
    char *rest, *token, *ptr;

    // file data
    float currentValue, minTemp, maxTemp, localMinTemp, localMaxTemp;
    float averageTemp = 0.0; // needs to be assigned to
    char *endptr;

    if(statsPrint == 0)
    {
        printf("\nrecieved index: %d\n", index);
    }

    // open file
    char *fileName = Concat(city, fileType);            // create complete file name
    char *filePath = Concat(fileDirectory, fileName);   // create path

    fd = fopen(filePath,"r"); // open file for reading

    // check error on file opening
    if(fd == NULL)
    {
        printf("error opening file\n");
        exit(1);
    }

    // read title line and ignore
    getline(&line, &lineLength, fd);
    linesRead++;
    
    // new block
    while((getlineResult = getline(&line, &lineLength, fd)) != -1) // while result of getline() is not -1
    {
        ptr = line;

        // loop for tokenize
        while(token = strtok_r(ptr, delim, &rest))
        {
            // cast to float
            currentValue = strtof(token, &endptr);

            averageTemp += currentValue;

            if(isMaxColumn)
            {
                // if reading first line of values
                if(linesRead < 2) { maxTemp = currentValue; }

                if(currentValue > maxTemp)
                {
                    maxTemp = currentValue; 
                    
                    // critical section begin
                    pthread_mutex_lock(&max_mutex); 
                    if (maxTemp > globalMaxTemp)
                    {
                        globalMaxTemp = maxTemp;
                    }
                    pthread_mutex_unlock(&max_mutex); 
                    // critical section end
                }
            }
            else
            {
                // if reading first line of values
                if(linesRead < 2) { minTemp = currentValue; }

                if(currentValue < minTemp)
                {
                    minTemp = currentValue;
                    
                    // critical section begin
                    pthread_mutex_lock(&min_mutex); 
                    if (minTemp < globalMinTemp)
                    {
                        globalMinTemp = minTemp;
                    }
                    pthread_mutex_unlock(&min_mutex); 
                    // critical section end
                }
            }

            ptr = rest;
            // next column
            if(isMaxColumn) { isMaxColumn = 0; }
            else { isMaxColumn = 1; }
        }
        
        linesRead++; // TODO: don't increment on empty lines
    }

    // begin critical section
    pthread_mutex_lock(&lines_mutex); 
    globalLinesRead = globalLinesRead + linesRead;
    pthread_mutex_unlock(&lines_mutex); 
    // end critical section

    // begin critical section
    pthread_mutex_lock(&files_mutex);
    global_filesRead++;
    pthread_mutex_unlock(&files_mutex);
    // end critical section

    averageTemp = averageTemp / linesRead; // calculate average

    free(line); 

    if( statsPrint == 0)
    {
        PrintCityData(city, minTemp, maxTemp, averageTemp, linesRead);
    }
    
    return NULL;

}

int main(int argc, char *argv[])
{
    // set variables
    int fdList[NUM_OF_FILES];

    // check number of arguments
    if(argc > 2)
    {
        printf("Usage:\t%s (single threaded)\n\t%s -m (multi threaded)\n", argv[NAME_INDEX], argv[NAME_INDEX]);
        exit(1);
    }

    // if there is a option
    if(argc == 2)
    {
        if(strcmp(argv[1], "-m") == 0)
        {
            multithreading = 1;
        }
        
        if(strcmp(argv[1], "-p") == 0)
        {
           statsPrint = 1;
        }

        if(strcmp(argv[1], "-mp") == 0)
        {
           statsPrint = 1;
           multithreading = 1;
        }
        
    }

    // set up for threads
    pthread_t *threadID;
    threadID = (pthread_t *) malloc(sizeof(pthread_t) * NUM_OF_FILES);

    // open and process files
    for(int cityIndex = 0; cityIndex < NUM_OF_FILES; cityIndex++)
    {
        int indexValue = cityIndex;

        if(multithreading) // multithreading mode
        {
            if(statsPrint == 0)
            {
                printf("passing index: %d\n", indexValue);
            }

            // create threads here
            // pass the address of an int, cast to a void pointer
            pthread_create(&threadID[cityIndex], NULL, ProcessFile, (void*)&indexValue);
        }
        else // if in linear mode
        {
            ProcessFile(&indexValue);
        }
    }

    // synchronize join of all files
    if(multithreading)
    {
        for(int i = 0; i < NUM_OF_FILES; i++)
        {
            pthread_join(threadID[i], NULL);
        }
    }

    clock_t programClock = clock();

    if(statsPrint == 0)
    {
        // print global data
        PrintSeparator(40);
        printf("\nTotal lines read: %li", globalLinesRead);
        printf("\nHighest temperature overall: %f", globalMaxTemp);
        printf("\nLowest temperature overall: %f", globalMinTemp);
        printf("\nElapsed time: %li\n", programClock);
        printf("\nTotal files read: %d\n", global_filesRead);
    }
    else
    {
        printf("%li\n", programClock);
    }

    return 0;
}
