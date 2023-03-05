#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
// add more includes

#define NUM_OF_FILES 10

// function prototypes
void PrintCityData();

char *Concat(char *string1, char *string2); 

void ProcessFile(char *fileName);

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

void ProcessFile(char *fileName)
{   
    int fd = open(fileName, O_RDWR | O_CREAT, 0777);

    if(fd == -1)
    {
        printf("error opening file\n");
        exit(1);
    }

    printf("Successfully ran ProcessFile\n");
}

int main(int argc, char *argv[])
{
    int multithreading = 0;
   
    // ----- for debugging -----
    printf("argc: %d\n", argc);
    printf("\n");
    for(int i = 0; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }
    printf("\n");
    // ------------------------
    
    if(argc > 2)
    {
        printf("Too many arguments\n");
        exit(1);
    }


    if(argc == 2)
    {
        if(strcmp(argv[1], "-m") == 0)
        {
            multithreading = 1;
            printf("running with multithreading option\n");
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
    char *filepath = "data_files";
    int fdList[NUM_OF_FILES];

    //print file names
    for(int i = 0; i < NUM_OF_FILES; i++)
    {
        printf("%s\n", fileNames[i]);
    }
    
    // open file
    for(int i = 0; i < NUM_OF_FILES; i++)
    {
        char *fileName = Concat(filepath, fileNames[i]);
        ProcessFile(fileName);
    }

    return 0;
}
