#ifndef TEAMPROJ_H_INCLUDED
#define TEAMPROJ_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//FCFS

typedef struct queue{
    struct queue * next;
    char name[20];
    int arrival_time;
    int burst_time;
    int cpu_time;

}node;

//RR

#define robintime 20

typedef struct Data_2{
    char name[10];
    int arrivaltime;
    int bursttime;
    int waitingtime;
	int checktime;
    struct Data_2 * next;
}data_2;

typedef struct{
    int count;
    data_2 * front;
    data_2 * rear;
}qeType_2;

//SJF_nonpreemptive

typedef struct mytype
{
    char name[20];
    int artime;
    int brustime;
    int cputime;
    struct mytype *next;
} process;

//SJF_preemptive

typedef struct Data{
    char name[10];
    int arrivaltime;
    int bursttime;
    int waitingtime;
    struct Data * next;
}data;

typedef struct{
    int count;
    data * front;
    data * rear;
}qeType;

//


#endif // TEAMPROJ_H_INCLUDED
