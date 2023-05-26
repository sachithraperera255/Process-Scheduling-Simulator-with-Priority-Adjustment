#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>

// #define SIZE BUFSIZ
#define NUM_THREADS 4

void *consumer(void *arg);
void *producer(void *arg);

// this buffer handle process data
struct task_buffer
{
    char process_type[10];
    int static_priority_value;
    int execution_time;

    int dynamic_time_silce;
    int dynamic_priority;
    int sleep_avg;
    int time_slice; // also called quantum time
};
struct request_q
{
    struct task_buffer qs[20];
};

// this struct is for file handling and sorting purpose
struct file_info
{
    char process_name[10];
    int priority;
    int time;
};

// initializing array of struc
struct file_info File_arr[20];
struct request_q rq[3];

// keep track of number of lines read from the fileheader.h
int count = 0;

// initializing local array for purpise of reading data.txt
char Local_arr[20];

// bubble sort function, descending
void bubbleSort(struct file_info *File_arr, int len);

// calculating the size of the array.
int size_of_File_arr = sizeof(File_arr) / sizeof(File_arr[0]);
int size_of_Local_arr = sizeof(Local_arr) / sizeof(Local_arr[0]);
int size_of_rq = sizeof(rq) / sizeof(rq[0]);

void bubbleSortRq(struct request_q *rq, int len);

int main()
{
    int resConsumer, resProducer;
    int lots_of_threads;
    pthread_t a_thread[NUM_THREADS];
    void *threadConsumer_results;

    // initializing token
    char *item;

    // initializing file handler
    FILE *fp;

    // file opening and error cheking
    fp = fopen("data.txt", "r");
    if (fp == NULL)
    {
        perror("Error opening files\n");
        exit(EXIT_SUCCESS);
    }

    // reading the file untill the end and use "strtok" to extract the info seperately.
    while (fgets(Local_arr, size_of_Local_arr, fp) != NULL)
    {
        item = strtok(Local_arr, ",");
        strcpy(File_arr[count].process_name, item);

        item = strtok(NULL, ",");
        File_arr[count].priority = atoi(item);

        item = strtok(NULL, " ");
        File_arr[count].time = atoi(item);

        count++;
    }

    // closing the file and checking for error
    if (fclose(fp) != 0)
    {
        perror("Error closing the file\n");
        exit(EXIT_FAILURE);
    }

    printf("Before sorting\n");
    for (int i = 0; i < size_of_File_arr; i++)
    {
        printf("%s %d %d\n", File_arr[i].process_name, File_arr[i].priority, File_arr[i].time);
    }

    /*
     * calling bubbleSort to sort the File_arr[]
     */
    bubbleSort(File_arr, size_of_File_arr);

    printf("\n");

    printf("After Sorting\n");
    for (int i = 0; i < count; i++)
    {
        if (File_arr[i].priority > 0)
        {
            printf("%s %d %d\n", File_arr[i].process_name, File_arr[i].priority, File_arr[i].time);
        }
    }

    // creating producer thread
    resProducer = pthread_create(&(a_thread[lots_of_threads]), NULL, producer, (void *)&lots_of_threads);
    if (resProducer != 0)
    {
        perror("Thread create failed");
    }

    // creating consumer threads
    for (lots_of_threads = 0; lots_of_threads < NUM_THREADS; lots_of_threads++)
    {
        resConsumer = pthread_create(&(a_thread[lots_of_threads]), NULL, consumer, (void *)&lots_of_threads);
        if (resConsumer != 0)
        {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }

    // consumer threads joining
    for (lots_of_threads = NUM_THREADS - 1; lots_of_threads >= 0; lots_of_threads--)
    {
        resConsumer = pthread_join(a_thread[lots_of_threads], &threadConsumer_results);
        if (resConsumer == 0)
        {
            printf("Picked up a thread\n");
        }
        else
        {
            perror("pthread_join failed");
        }
    }

    printf("All done\n");
    exit(EXIT_SUCCESS);
}

void *producer(void *arg)
{
    int i = 0;
    int k = 0;
    int l = 0;
    int m = 0;

    // keep track of number of element put into the rqs
    int count_rq0 = 0;
    int count_rq1 = 0;
    int count_rq2 = 0;

    printf("This is Producer\n");

    // loop runs until i is < numbers of line read and loop through "File_arr array" array of structure
    for (k = 0; k < size_of_File_arr; k++)
    {

        if (0 <= File_arr[k].priority && File_arr[k].priority <= 99)
        {
            while (l <= count_rq0)
            {
                memcpy(&rq[0].qs[l], &File_arr[k], sizeof(File_arr[k]));
                l++;
            }

            count_rq0++;
        }

        if (100 <= File_arr[k].priority && File_arr[k].priority <= 129)
        {
            while (m <= count_rq1)
            {
                memcpy(&rq[1].qs[m], &File_arr[k], sizeof(File_arr[k]));
                m++;
            }
            count_rq1++;
        }

        if (130 <= File_arr[k].priority && File_arr[k].priority <= 139)
        {
            while (i <= count_rq2)
            {
                memcpy(&rq[2].qs[i], &File_arr[k], sizeof(File_arr[k]));
                i++;
            }
            count_rq2++;
        }
    }

    for (int x = 0; x < 3; x++)
    {
        for (int k = 0; k < 20; k++)
        {
            if(rq[x].qs[k].static_priority_value > 0)
            {
                if(x == 0)
                {
                    printf("Distributed data in RQ%d: %s %d %d\n",x ,rq[x].qs[k].process_type, rq[x].qs[k].static_priority_value, rq[x].qs[k].execution_time);
                }else if (x == 1)
                {
                    printf("Distributed data in RQ%d: %s %d %d\n",x ,rq[x].qs[k].process_type, rq[x].qs[k].static_priority_value, rq[x].qs[k].execution_time);
                }else if (x == 2)
                {
                    printf("Distributed data in RQ%d: %s %d %d\n",x ,rq[x].qs[k].process_type, rq[x].qs[k].static_priority_value, rq[x].qs[k].execution_time);
                }
                
            }
            
        }
    }

    int rand_num = 1 + (int)(9.0 * rand() / (RAND_MAX + 1.0));
    sleep(rand_num);
    pthread_exit(NULL);
}

void *consumer(void *arg)
{
    {
        printf("\nThis is consumer\n");
        int id = *((int *)arg);
        srand(time(0));
        int time_provided_for_exe = ((rand() % 10 - 1 + 1) + 1) * 1000; // This is the time provided for process, x10000 to convert to milisecs.
        int bonus = (rand() % 10 - 1 + 1) + 1;
        int sleep_time = 1000;
        int time_s = 0; // time_slice
        int sa = 0;     // sleep_avg
        int k;
        int count = 0;
        int t1 = 0;
        int t2 = 0;
        int max_S = 10;
        int ticks = 0;
        int deltaD = 0;
        int temp_priority = 0;
        int dp = 0;

        struct timeval start, end;

        printf("time need for exe: %d\n", time_provided_for_exe);
        printf("id: %d\n", id);

        for (int z = 0; z <= id; z++)
        {
            for (int i = 0; i < size_of_rq; i++)
            {
                for (int j = 0; j < 20; j++)
                {
                    k = rq[i].qs[j].static_priority_value; // here we stop non assign (zeros) values in the arrays entering the process
                    if (rq[i].qs[j].static_priority_value > 0)
                    {

                        if (strcmp(rq[i].qs[j].process_type, "FIFO") == 0)
                        {

                            rq[i].qs[j].time_slice = rq[i].qs[j].execution_time;
                            // printf("Execution time of %s is %d (ms)\n", rq[i].qs[j].process_type, rq[i].qs[j].dynamic_time_silce);
                        }

                        if (strcmp(rq[i].qs[j].process_type, "RR") == 0)
                        {

                            rq[i].qs[j].time_slice = time_provided_for_exe;

                            if (rq[i].qs[j].time_slice == 0)
                                continue;
                            if (rq[i].qs[j].execution_time < rq[i].qs[j].time_slice)
                            {
                                usleep(sleep_time);
                                // printf("Blocked process: %s %d %d\n", rq[i].qs[j].process_type, rq[i].qs[j].static_priority_value, rq[i].qs[j].time_slice);
                            }
                            else if (rq[i].qs[j].execution_time == rq[i].qs[j].time_slice)
                            {
                                // printf("Executed process: %s %d %d\n", rq[i].qs[j].process_type, rq[i].qs[j].static_priority_value, rq[i].qs[j].time_slice);
                            }
                        }

                        if (strcmp(rq[i].qs[j].process_type, "NORMAL") == 0)
                        {
                            rq[i].qs[j].time_slice = time_provided_for_exe;

                            if (rq[i].qs[j].time_slice == 0)
                                continue;
                            if (rq[i].qs[j].execution_time < rq[i].qs[j].time_slice)
                            {
                                t1 += gettimeofday(&start, NULL);
                                usleep(sleep_time);
                                t2 += gettimeofday(&end, NULL);
                                int deltaT = t2 - t1;
                                ticks = ticks + (deltaT / 100);

                                deltaD = deltaD + deltaT - ticks;
                            }
                            if (rq[i].qs[j].execution_time > rq[i].qs[j].time_slice) // process become runnable
                            {
                                for (int l = ticks; l < max_S; l++)
                                {
                                    ticks += l;
                                }
                                rq[i].qs[j].sleep_avg = ticks;
                                // printf("sleeping avg of runnable process: %d\n", rq[i].qs[j].sleep_avg);

                                rq[i].qs[j].dynamic_priority = rq[i].qs[j].static_priority_value;

                                dp = fmax(100, fmin(rq[i].qs[j].dynamic_priority - bonus + 5, 139));

                                rq[i].qs[j].dynamic_priority = dp;
                            }

                            // printf("previous priority: %s %d\n", rq[i].qs[j].process_type, rq[i].qs[j].static_priority_value);
                            // printf("updated priority value: %s %d\n", rq[i].qs[j].process_type, rq[i].qs[j].dynamic_priority);
                        }
                    }
                }
            }
        }
    }

    // *** need to sort rqs in ascending order for next process. make a fucntion outside of the thread and call the function here ***

    printf("\n");

    for (int rq_num = 0; rq_num < 3; rq_num++)
    {
        bubbleSortRq(&rq[rq_num], 20);

        for (int m = 0; m < 20; m++)
        {
            if (rq[rq_num].qs[m].dynamic_priority > 0)
            {
                printf("sorted RQ%d: %s %d %d\n", rq_num, rq[rq_num].qs[m].process_type, rq[rq_num].qs[m].dynamic_priority, rq[rq_num].qs[m].time_slice);
            }
        }
    }

    int rand_num = 1 + (int)(9.0 * rand() / (RAND_MAX + 1.0));
    sleep(rand_num);
    pthread_exit(NULL);
}

// bubble sort function, descending
void bubbleSort(struct file_info *File_arr, int len)
{
    printf("file sort begin\n");
    int i, j, is_swapped;
    struct file_info temp;

    for (i = 0; i < len; i++)
    {
        is_swapped = 0;

        for (j = 0; j < (len - 1 - i); j++)
        {
            if (File_arr[j].priority > File_arr[j + 1].priority)
            {
                temp = File_arr[j];
                File_arr[j] = File_arr[j + 1];
                File_arr[j + 1] = temp;

                is_swapped = 1;
            }
        }

        if (!is_swapped)
        {
            break;
        }
    }
    printf("file sort end\n");
}

// bubble sort for rqs after process

void bubbleSortRq(struct request_q *rq, int len)
{
    printf("file sort begin\n");
    int i, j, is_swapped;
    struct request_q temp;

    for (i = 0; i < len; i++)
    {
        is_swapped = 0;

        for (j = 0; j < (len - 1 - i); j++)
        {
            if (rq[j].qs->dynamic_priority < rq[j + 1].qs->dynamic_priority)
            {
                temp = rq[j];
                rq[j] = rq[j + 1];
                rq[j + 1] = temp;

                is_swapped = 1;
            }
        }

        if (!is_swapped)
        {
            break;
        }
    }
    printf("file sort end\n");
}
