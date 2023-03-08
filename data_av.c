#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

#define NUM_OF_FILES 10
#define NAME_INDEX 0
#define OPTION_INDEX 1

// function prototypes

void PrintCityData(char *city, float minTemp, float maxTemp, float averageTemp, int linesProcessed);

char *Concat(char *string1, char *string2); 

void ProcessFile(char *filePath);

// Print data gathered and calculated for each city file.
// TODO: Could use a struct as the argument.
void PrintCityData(char *city, float minTemp, float maxTemp, float averageTemp, int valuesProcessed)
{
    // print separator
    int count = 40;
    for(int i = 1; i <= count; i++)
    {
        printf("=");
    }

    // Assignment output
    printf("\nData for: %s city", city);
    printf("\n%s's highest temperature: %2.3f", city, maxTemp);
    printf("\n%s's lowest temperature: %2.3f", city, minTemp);
    printf("\n%s's average temperature: %2.3f", city, averageTemp);
    printf("\nTotal values processed for %s is: %d", city, valuesProcessed);
    printf("\n\n");
    
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
// TODO: could this function be broken down?
void ProcessFile(char *filePath)
{
    //printf("opening: %s\n", filePath);

    FILE *fd;
    char *line = NULL; // line we read from the file
    size_t lineLength = 0;
    ssize_t getlineResult;
    int linesRead = 0; // record how many lines of the file we have gotten
    int currentLine = 1; // record which line of the file we are getting
    long valuesProcessed = 0;
   
    // for strtok_r
    const char *delim = "\t";
    char *rest, *token, *ptr;
    float currentValue, minTemp, maxTemp, localMinTemp, localMaxTemp, averageTemp;
    char *endptr;
    int isMaxColumn = 1; // indicates if the current token is from the max column (first column)
    
    fd = fopen(filePath,"r"); // "r" means open file for reading

    // check error on file opening
    if(fd == NULL)
    {
        printf("error opening file\n");
        exit(1);
    }

    // read title line and ignore
    getline(&line, &lineLength, fd);
    linesRead++;

    averageTemp = 0.0; // needs to be assigned
    
    // new block
    while((getlineResult = getline(&line, &lineLength, fd)) != -1) // while result of getline() is not -1
    {
        ptr = line;

        // loop for tokenize
        while(token = strtok_r(ptr, delim, &rest))
        {
            valuesProcessed++;
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
            if(isMaxColumn)
            {
                isMaxColumn = 0;
            }
            else
            {
                isMaxColumn = 1;
            }
        }
        
        linesRead++;
    }

    averageTemp = averageTemp / linesRead; // calculate average

    free(line); 

    PrintCityData(filePath, minTemp, maxTemp, averageTemp, valuesProcessed);

}

int main(int argc, char *argv[])
{
    // set variables
    int multithreading = 0;
    int fdList[NUM_OF_FILES];
    char *filepath = "data_files/";

    // indecies of filenames in array
    enum city
    {
        Mont = 0,
        Hal = 1,
        Edm = 2,
        Charl = 3,
        Ott = 4,
        Qu = 5,
        Tor = 6,
        Van = 7,
        Vic = 8,
        Win = 9,
    };

    char *fileNames[NUM_OF_FILES] = 
    {
        "Montreal.dat",
        "Halifax.dat",
        "Edmonton.dat",
        "Charlottetown.dat", 
        "Ottawa.dat",
        "Quebec.dat",
        "Toronto.dat",
        "Vancouver.dat",
        "Victoria.dat",
        "Winnipeg.dat"
    };

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

  
    // Non-multithreading
    // open and process files
    for(int i = 0; i < NUM_OF_FILES; i++)
    {
        char *fileName = Concat(filepath, fileNames[i]); // create path

        // TODO: add multithreading
        // this function is probably the one which will be called in pthread()
        ProcessFile(fileName);
    }

    return 0;
}
