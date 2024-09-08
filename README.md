# Principles_of_Operating-Systems-HW2
This repository contains two C programs that read numbers from a file, compute their sum, and measure the execution time. The first program uses a single thread, while the second leverages multi-threading with optional mutex-based synchronization for thread-safe updates.

1. Single-Threaded Sum Program:

This program reads integers from a file into an array, computes the sum of the values, and displays the result along with the execution time in milliseconds. It uses the clock() function for time measurement and handles basic file reading errors.
Multi-Threaded Sum Program:

This program performs the same summation task using multiple threads, dividing the array into slices for each thread. Optionally, a mutex lock is employed to ensure thread-safe updates to the total sum. The program takes three command-line arguments: the number of threads, the file name, and a flag indicating whether to use the mutex lock. Execution time is measured using the gettimeofday() function, and the result is displayed along with the total execution time.
Both programs handle file reading, error checking, and summation, with the second program adding support for parallel execution to improve performance on large datasets.

2. Multi-Threaded Sum Program:

This program performs the same summation task using multiple threads, dividing the array into slices for each thread. Optionally, a mutex lock is employed to ensure thread-safe updates to the total sum. The program takes three command-line arguments: the number of threads, the file name, and a flag indicating whether to use the mutex lock. Execution time is measured using the gettimeofday() function, and the result is displayed along with the total execution time.

Both programs handle file reading, error checking, and summation, with the second program adding support for parallel execution to improve performance on large datasets.
