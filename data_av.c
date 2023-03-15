#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define NUM_OF_FILES 10
#define NAME_INDEX 0
#define OPTION_INDEX 1

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// This is global now
char *fileNames[NUM_OF_FILES] = 
{
    // alphabetical order
    "Charlottetown.dat", 
    "Edmonton.dat",
    "Halifax.dat",
    "Montreal.dat",
    "Ottawa.dat",
    "Quebec.dat",
    "Toronto.dat",
    "Vancouver.dat",
    "Victoria.dat",
    "Winnipeg.dat"
};

// indecies of filenames in array
enum city
{
    Charl = 0,
    Edm = 1,
    Hal = 2,
    Mont = 3,
    Ott = 4,
    Qu = 5,
    Tor = 6,
    Van = 7,
    Vic = 8,
    Win = 9,
};

// Structs for storing data from thread
struct cityData 
{
    char *city;
    int totalValues;
    float maxTemp;
    float minTemp;
    int elapsedTime;
};

struct summaryData 
{
    int totalValues;
    float overalMaxTemp;
    char *maxTempCity;
    float overallMinTemp;
    char *minTempCity;
    int elapsedTime;
};

// function prototypes

void PrintCityData(char *city, float minTemp, float maxTemp, float averageTemp, int linesProcessed);

char *Concat(char *string1, char *string2); 

void *ProcessFile(void *cityIndex);

// Print data gathered and calculated for each city file.
void PrintCityData(char *city, float minTemp, float maxTemp, float averageTemp, int linesProcessed)
{
    char buffer[128];
    char *a, *b, *c;

    // print separator
    int count = 40;
    for(int i = 1; i <= count; i++)
    {
        printf("=");
    }
    
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


// open and read file, and calculate required values
void *ProcessFile(void *cityIndex)
{
    // reading files
    FILE *fd;
    char *fileDirectory= "data_files/";     // location of files
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

    // TODO: might need a lock here
    // handle argument
    // cityIndex is actually an address (specifically a void pointer), but we want to make it an int. 
    // First cast cityIndex to an int pointer, then access it's value (an address).
    int index = *(int *)cityIndex;
    printf("recieved index: %d\n", index);

    // open file
    // possible critical section
    pthread_mutex_lock(&mutex);
    char *filePath = Concat(fileDirectory, fileNames[index]); // create path
    pthread_mutex_unlock(&mutex);

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
                
                if(linesRead < 2) // if reading first line of values
                {
                    maxTemp = currentValue; 
                }

                if(currentValue > maxTemp)
                {
                    maxTemp = currentValue; 
                }
            }
            else
            {
                
                if(linesRead < 2) // if reading first line of values
                {
                    minTemp = currentValue; 
                }

                if(currentValue < minTemp)
                {
                    minTemp = currentValue;
                }
            }

            ptr = rest;
            // next column
            if(isMaxColumn)
            {
                isMaxColumn = 0;
            }
            else
            {
                isMaxColumn = 1;
            }
        }
        
        linesRead++; // TODO: don't increment on empty lines
    }

    averageTemp = averageTemp / linesRead; // calculate average

    free(line); 

    // TODO: made change from linesRead to linesProcessed
    PrintCityData(filePath, minTemp, maxTemp, averageTemp, linesRead);

    return NULL;

}

int main(int argc, char *argv[])
{
    // set variables
    int multithreading = 0;
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
        if(strcmp(argv[OPTION_INDEX], "-m") == 0)
        {
            multithreading = 1;
            printf("running with multithreading option\n\n");
        }
        else
        {
            printf("option '%s' not recognized\n", argv[OPTION_INDEX]);
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
            // create threads here
            // pass the address of an int, cast to a void pointer
            printf("passing index: %d\n", indexValue);
            pthread_create(&threadID[cityIndex], NULL, ProcessFile, (void*)&indexValue);
        }
        else // if in linear mode
        {
            ProcessFile(&indexValue);
            printf("linear call\n");
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
    printf("\n%li\nProgram finished.\n", programClock);

    return 0;
}
