#ifndef TEAMPROJ_H_INCLUDED
#define TEAMPROJ_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 100

//        FCFS         //
//        Linked List
typedef struct queue{
    struct queue * next;
    char name[20];
    int arrival_time;
    int burst_time;
    int cpu_time;

}node;

//        Array
typedef struct Queue{

    char name[202][5];
    int arraival_time[202];
    int burst_time[202];
    int rear;
    int front;
    int cpu_time[202];


}queue;

//        RR         //
#define robintime 20

//        Linked List

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

//        Array
typedef struct{

    char name[100][10];
    int arrivaltime[100];
    int bursttime[100];
    int waitingtime[100];
	int checktime[100];
	int count;
    int front;
    int rear;
}data_RR_Array;

//        SJF nonpreemptive         //

typedef struct mytype
{
    char name[20];
    int artime;
    int brustime;
    int cputime;
    struct mytype *next;
} process;

//        Array
typedef struct
{
    char *name;
    int artime;
    int brustime;
    int cputime;
} process_Array;


//        SJF preemptive         //
//        Linked List
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

//        Array
typedef struct{
    char name[10];
    int arrivaltime;
    int bursttime;
    int waitingtime;
}data_Array_SJFpre;

typedef struct{
    int count;
    int front;
    int rear;
    data_Array_SJFpre * Array;
}qeType_Array_SJFpre;


//


#endif // TEAMPROJ_H_INCLUDED
