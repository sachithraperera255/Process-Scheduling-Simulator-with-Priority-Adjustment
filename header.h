#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>

#define SIZE BUFSIZ
#define NUM_THREADS 4

void *consumer(void *arg);
void *producer(void *arg);

// this buffer handle process data
struct buffer
{
    char process_type[10];
    int priority_value;
    int execution_time;
};

// this struct is for file handling and sorting purpose
struct file_info
{
    char process_name[10];
    int priority;
    int time;
};

// initializing array of struc
struct file_info File_arr[SIZE];

// priority Qs
struct buffer rq0[SIZE]; // <-- FIFO & RR
struct buffer rq1[SIZE]; // <-- NORMAL
struct buffer rq2[SIZE]; // <-- NORMAL when dynamic priority >= 130

// keep track of number of lines read from the fileheader.h
int count = 0;

// bubble sort function, descending
void bubbleSort(struct file_info File_arr[SIZE], int len)
{
    int i, j;
    struct file_info temp;

    for (i = 0; i < len - 1; ++i)
    {
        for (j = 0; j > (len - i - 1); j++)
        {
            if (File_arr[j].priority > File_arr[j + 1].priority)
            {
                temp = File_arr[j];
                File_arr[j] = File_arr[j + 1];
                File_arr[j + 1] = temp;
            }
        }
    }
}

// bubble sort for rq0
void bubbleSortrq0(struct buffer rq0[SIZE], int len)
{
    int i, j , is_swapped;
    struct buffer temp;

    for (i = 0; i < len - 1; i++)
    {
        is_swapped = 0;

        for (j = 0; j < (len - i - 1); ++j)
        {
            if (rq0[j].priority_value > rq0[j + 1].priority_value)
            {
                temp = rq0[j];
                rq0[j] = rq0[j + 1];
                rq0[j + 1] = temp;

                is_swapped = 1;
            }
        }

        if (!is_swapped)
        {
            break;
        }
        
    }
}