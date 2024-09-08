#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

#define MAXSIZE 1000001

int readFile(char[], int[]);
void* arraySum(void*);

typedef struct _thread_data_t{
    const int *data;         //Pointer to array of data read from file (ALL)
    int startInd;            //Starting index of thread's slice
    int endInd;              //Ending index of thread's slice
    long long int *totalSum; //Pointer to the total sum variable in main
    pthread_mutex_t *lock;   //Critical region lock
} thread_data_t;

int main(int argc, char* argv[]){
    long long int totalSum = 0;        //Holds the total sum
    struct timeval beginTime, endTime; //Measures execution Time
    double executionTime;              //Holds execution time in milliseconds

    //Checks for correct number of command line parameters
    if(argc != 4){
        printf("There are an incorrect number of parameters.\n");
        return -1;
    }

    //Parse command line parameter
    int numberThreads = atoi(argv[1]);
    char* fileName = argv[2];
    int lockInUse = atoi(argv[3]);

    //Reads the data from the file into the array
    int array[MAXSIZE];
    int numberValues = readFile(fileName, array);
    if(numberValues == -1){
        printf("Error reading file.\n");
        return -1;
    }

    //Checks if the number of threads requested is greater than the number of values
    if(numberThreads > numberValues){
        printf("Error: Too many threads requested.\n");
        return -1;
    }

    //Get the start time
    gettimeofday(&beginTime, NULL);

    pthread_mutex_t lock;

    //Initializes mutex lock if it's needed
    if(lockInUse){
        pthread_mutex_init(&lock, NULL);
    }

    pthread_t threads[numberThreads];         //An array that holds the thread IDs
    thread_data_t thread_data[numberThreads]; //An array that holds thread-specific data

    //Creation of the threads
    for(int i = 0; i < numberThreads; i++){
        thread_data[i].data = array;                      //Pointer to the data array
        int sizeOfArray = numberValues/numberThreads;     //Size of each of the thread's slice
        thread_data[i].startInd = i * sizeOfArray;        //Starting index of thread's slice
        //Ending index of thread's slice
        if(i == numberThreads - 1){
            thread_data[i].endInd = numberValues - 1;
        } else{
            thread_data[i].endInd = (i+1) * sizeOfArray - 1;
        }
        thread_data[i].totalSum = &totalSum;              //Pointer to the total sum variable
        thread_data[i].lock = (lockInUse) ? &lock : NULL; //Pointer to mutex lock if used

        //Creates thread with thread-specific data
        int returnCode = pthread_create(&threads[i], NULL, arraySum, (void*)&thread_data[i]);
        if(returnCode != 0){
            printf("There's been an error creating the thread: %d\n", returnCode);
            exit(EXIT_FAILURE);
        }
    }

    //Waits for all threads to finish
    for(int i = 0; i < numberThreads; i++){
        pthread_join(threads[i], NULL);
    }

    //Get the end time
    gettimeofday(&endTime, NULL);

    //Calculates the execution time in milliseconds
    executionTime = (double)(endTime.tv_sec - beginTime.tv_sec) * 1000.0;
    executionTime += (double)(endTime.tv_usec - beginTime.tv_usec)/1000.0;

    //Prints the total sum and execution time
    printf("The total sum is: %11lld\n", totalSum);
    printf("The total execution time is: %.3f milliseconds\n", executionTime);

    //Destroys the mutex lock if it is used
    if(lockInUse){
        pthread_mutex_destroy(&lock);
    }

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

void* arraySum(void* argument){
    //Cast the void pointer to thread_data_t pointer
    thread_data_t* data = (thread_data_t*) argument;
    long long int threadSum = 0;

    //Iterates over the assigned slice of the array and calculates the sum
    for(int i = data->startInd; i <= data->endInd; i++){
        threadSum += data->data[i];
    }

    //If a mutex lock is provided, acquire the lock, update the total sum, and release the lock 
    if(data->lock != NULL){
        pthread_mutex_lock(data->lock);
        *(data->totalSum) += threadSum;
        pthread_mutex_unlock(data->lock);
    } else{
        //If no lock is provided, update the total sum directly
        *(data->totalSum) += threadSum;
    }

    //Return NULL as required by the pthread_create function signature
    return NULL;

    return 0;
}