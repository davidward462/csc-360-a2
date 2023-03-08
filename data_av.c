#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define NUM_OF_FILES 10
#define NAME_INDEX 0
#define OPTION_INDEX 1

// function prototypes

void PrintCityData(char *city, float minTemp, float maxTemp, float averageTemp, int linesProcessed);

char *Concat(char *string1, char *string2); 

void ProcessFile(char *filePath);

// Print data gathered and calculated for each city file.
// Could use a struct as the argument.
void PrintCityData(char *city, float minTemp, float maxTemp, float averageTemp, int linesProcessed)
{
    // print separator
    int count = 40;
    for(int i = 1; i <= count; i++)
    {
        printf("=");
    }

    // Assignment output
    printf("\nData for: %s city", city);
    printf("\n%s's highest temperature: %f", city, maxTemp);
    printf("\n%s's lowest temperature: %f", city, minTemp);
    printf("\n%s's average temperature: %f", city, averageTemp);
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

void ProcessFile(char *filePath)
{
    printf("opening: %s\n", filePath);

    FILE *fd;
    char *line = NULL; // line we read from the file
    size_t lineLength = 0;
    ssize_t getlineResult;
    int linesRead = 0; // record how many lines of the file we have gotten
    int currentLine = 1; // record which line of the file we are getting
   
    // for strtok_r
    const char *delim = "\t";
    char *rest;
    char *token;
    char *ptr;

    float minColumnValue;
    float currentMinTemp;
    
    float maxColumnValue;
    float currentMaxTemp;

    float averageTemp;

    char *endptr;
    
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
    
    // new block
    while((getlineResult = getline(&line, &lineLength, fd)) != -1) // while result of getline() is not -1
    {
        ptr = line;
        // loop for tokenize
        while(token = strtok_r(ptr, delim, &rest))
        {
            printf("%s\n", token);
            ptr = rest;
        }
    }

    /**
    // read each line 
    while((getlineResult = getline(&line, &lineLength, fd)) != -1) // while result of getline() is not -1
    {
        printf("begin while (lines read: %d, current line: %d)\n", linesRead, linesRead+1); 
        // parse line
        // get first token (should be max)
        token = strtok_r(line, delim, &rest);

        printf("ran strtok_r");

        // check if line is empty
        if(token == NULL)
        {
            printf("empty line\n");
            break;
        }

        maxColumnValue = strtof(token, &endptr);

        // if this is the first line with values to read
        if(linesRead == 0)
        {
            currentMaxTemp = maxColumnValue;
        }

        if(maxColumnValue > currentMaxTemp)
        {
            // overwrite if a new max is found
            currentMaxTemp = maxColumnValue;
            printf("%2.1f ", currentMaxTemp); // for testing
        }

        // parse rest of token (should be min)
        // There is only ever one delim character, so no reason to make a loop
        token = strtok_r(NULL, delim, &rest);
        minColumnValue = strtof(token, &rest); 

        // if this is the first line with values to read
        if(linesRead == 0)
        {
            currentMinTemp = minColumnValue;
        }   

        if(minColumnValue < currentMinTemp)
        {
            currentMinTemp = minColumnValue;
        }

        linesRead++;
    }
    **/

    printf("\n");

    free(line); 

    printf("lines read: %d\n", linesRead);
    
    printf("\n");

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

    // for testing
    PrintCityData("Victoria", -3.4, 35.7, 10.1, 29012);

    return 0;
}
