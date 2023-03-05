#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define NUM_OF_FILES 10
#define NAME_INDEX 0
#define OPTION_INDEX 1

// function prototypes
void PrintCityData();

char *Concat(char *string1, char *string2); 

void ProcessFile(char *filePath);

// draft, will have parameters later
void PrintCityData()
{
    // print separator
    int count = 40;
    for(int i = 1; i <= count; i++)
    {
        printf("=");
    }

    // assign data
    // temporary
    char *cityName = "city";
    float minTemp = -20.3;
    float maxTemp = 36.1;
    float averageTemp = 21.0;
    int valuesProcessed = 1000;

    printf("\nData for: %s", cityName);
    printf("\n%s's lowest temperature: %f", cityName, minTemp);
    printf("\n%s's hightest temperature: %f", cityName, minTemp);
    printf("\nAverage temperature for %s: %f", cityName, averageTemp);

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
    printf("opening %s\n", filePath);

    FILE *fd;
    char *line = NULL;
    size_t lineLength = 0;
    ssize_t getlineResult;
    int linesRead = 0;
    
    fd = fopen(filePath,"r");

    // check error on file opening
    if(fd == NULL)
    {
        printf("error opening file\n");
        exit(1);
    }

    // read each line 
    while((getlineResult = getline(&line, &lineLength, fd)) != -1) // while result of getline() is not -1
    {
        //printf("%s", line);
        linesRead++;
    }
    free(line); 

    printf("lines read: %d\n", linesRead);
    
    printf("\n");

}

int main(int argc, char *argv[])
{
    int multithreading = 0;
   
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
            printf("running with multithreading option\n");
        }
        else
        {
            printf("option '%s' not recognized\n", argv[OPTION_INDEX]);
        }
    }

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

    char *fileNames[NUM_OF_FILES] = {"Montreal.dat","Halifax.dat","Edmonton.dat","Charlottetown.dat", "Ottawa.dat","Quebec.dat","Toronto.dat","Vancouver.dat","Victoria.dat", "Winnipeg.dat"};
    char *filepath = "data_files/";
    int fdList[NUM_OF_FILES];
    
    // open file
    for(int i = 0; i < NUM_OF_FILES; i++)
    {
        char *fileName = Concat(filepath, fileNames[i]);
        ProcessFile(fileName);
    }

    return 0;
}
