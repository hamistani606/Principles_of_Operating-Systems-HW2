#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAXSIZE 1000001

int readFile(char* fileName, int array[]);
int sumArray(int array[], int numberValues);

int main(int argc, char* argv[]){
    char* fileName;     //Stores file name
    int array[MAXSIZE]; //Array that stores numbers read from the file
    int numberValues;   //Number of values read from file
    int sum = 0;        //Stores the sum of numbers
    double totalTime;   //Total time taken

    //Checks if the correct number of arguments are provided
    if(argc !=2){
        printf("There are not enough parameters...\n");
        return 1;
    }
    
    //Gets the file name from the command line arguments
    fileName = argv[1];

    //Reads the numbers from the file into the array
    numberValues = readFile(fileName, array);

    //Checks if the file reading was successful 
    if(numberValues == -1){
        return 1;
    }

    //Starts measuring time
    clock_t begin = clock();

    //Calculates the sum of the numbers in the array
    sum = sumArray(array, numberValues);

    //Stops measuring time
    clock_t end = clock();

    //Calculates the total time taken
    totalTime = (double)(end - begin) / CLOCKS_PER_SEC;
    totalTime *= 1000;

    //Prints the sum of the numbers and the total time taken
    printf("The total sum: %d\n", sum);
    printf("The time taken: %.3f milliseconds\n", totalTime);

    return 0;
}

//Function that reads the nmbers from a file into an array
int readFile(char* fileName, int array[]){
    FILE* filePointer = fopen(fileName, "r"); //Opens the file
    int count = 0;                            //Variable to count the number of values  

    //Checks if the file could be opened
    if(filePointer == NULL){
        printf("This file could not be found...\n");
        return -1; //Returns -1 for failure to open
    }

    //Reads numbers from the file into the array until the end of the file is reached
    while(count < MAXSIZE && fscanf(filePointer, "%d", &array[count]) == 1){
        count++; //Increments cound to each value that is read
    }

    fclose(filePointer); //Closes file
    return count;        //Returns the number of values that are read
}


//Function that calculates the sum of numbers in an array
int sumArray(int array[], int numberValues){
    int sum = 0; //Initializes sum to zero

    //Iterates through the array and adds each element ot the sum
    for(int i = 0; i < numberValues; i++){
        sum += array[i];
    }

    return sum; //Returns the total sum 
}