#include "teamproj.h"


void readdata_Array_SJFpre(qeType_Array_SJFpre **, FILE *);
qeType_Array_SJFpre * newqueue_Array_SJFpre(void);
int enqueue_sortarrival_Array_SJFpre(qeType_Array_SJFpre ** );
int enqueue_sortburst_SJFpre(qeType_Array_SJFpre ** );
int enqueue_nonsort_Array_SJFpre(qeType_Array_SJFpre ** );
data_Array_SJFpre * dequeue_Array_SJFpre(qeType_Array_SJFpre ** );
void checkT_Array_SJFpre(qeType_Array_SJFpre ** , qeType_Array_SJFpre **);
void waitT_Array_SJFpre(qeType_Array_SJFpre ** , qeType_Array_SJFpre **);
void workQ_Array_SJFpre(qeType_Array_SJFpre ** , qeType_Array_SJFpre **);
void changedata(data_Array_SJFpre [],int ,int );

int SJF_preemptive_Array(FILE * fp)
{
    int i;

    qeType_Array_SJFpre * jobQ, * readyQ, *recordQ;
    jobQ = newqueue_Array_SJFpre();
    readyQ = newqueue_Array_SJFpre();
    recordQ = newqueue_Array_SJFpre();
    readdata_Array_SJFpre(&jobQ,fp);                                   // read data to jobQ, and sort it by arrival time
    checkT_Array_SJFpre( (&jobQ), (&readyQ));                          // check jobQ's arrival time = 0 , if = 0 to readyQ, and sort it by burst time
    while( (jobQ->count != 0) || (readyQ->count != 0) ){
        if( (readyQ->count) != 0){
            workQ_Array_SJFpre( (&readyQ), (&recordQ) );               // readyQ's first burst time - 1, if burst time = 0 to recordQ, and sort it by name, others waiting time + 1
        }
        waitT_Array_SJFpre( (&jobQ), (&readyQ));                       // jobQ's arrival time - 1, if arrival time = 0, data to readyQ, and sort it by burst time
    }

    double sum = 0.0;
    printf("----------\n");
    i = recordQ->front;
    while(i < (recordQ->count)){
        printf("%s  %d\n" ,(recordQ->Array[i].name), (recordQ->Array[i].waitingtime));
        sum = sum + recordQ->Array[i].waitingtime;
        i++;
    }
    printf("----------\n\n");
    printf("Average waiting time: %f\n\n\n",(sum/recordQ->count) );


    return 0;
}

void readdata_Array_SJFpre(qeType_Array_SJFpre ** Q, FILE * fp)                   // read data to jobQ, and sort it by arrival time
{
    int i=0;
    while( (i < SIZE) && fscanf(fp,"%s %d %d" ,(*Q)->Array[i].name ,&((*Q)->Array[i].arrivaltime) ,&((*Q)->Array[i].bursttime) ) != EOF ){
        (*Q)->Array[i].waitingtime = 0;
        (*Q)->front = 0;
        enqueue_sortarrival_Array_SJFpre(Q);
        i++;
    }
    if(i >= SIZE){
        printf("Too much data");
    }
    return;
}

qeType_Array_SJFpre * newqueue_Array_SJFpre(void)                                 // create a new queue
{
    qeType_Array_SJFpre * Q;
    Q = malloc(sizeof(qeType_Array_SJFpre));
    Q->count = 0;
    Q->front = 0;
    Q->rear = 0;
    Q->Array = malloc(SIZE * sizeof(data_Array_SJFpre)) ;
    return Q;
}

int enqueue_sortarrival_Array_SJFpre(qeType_Array_SJFpre ** Q)        // sort data by arrival time
{
    int i;
    if( (*Q)->count == 0){
        (*Q)->count++;
    }
    else{
        (*Q)->rear++;
        (*Q)->count++;
    }
    i = (*Q)->rear;
    while(i > 0){
            if( (*Q)->Array[i].arrivaltime < (*Q)->Array[i-1].arrivaltime ){
                changedata( ((*Q)->Array), i, (i-1) );
            }
            else{
                return 1;
            }
            i--;
    }

    return 0;
}

int enqueue_sortburst_SJFpre(qeType_Array_SJFpre ** Q)        // sort data by burst time
{
    int i;
    if( (*Q)->count == 0){
        (*Q)->count++;
    }
    else{
        (*Q)->rear++;
        (*Q)->count++;
    }
    i = (*Q)->rear;

    while(i > 0){
            if( (*Q)->Array[i].bursttime < (*Q)->Array[i-1].bursttime ){
                changedata( ((*Q)->Array), i, (i-1) );
            }
            else{
                return 1;
            }
            i--;
    }
    return 0;
}

int enqueue_nonsort_Array_SJFpre(qeType_Array_SJFpre ** Q)       // sort data by name
{
    int i;
    if( (*Q)->count == 0){
        (*Q)->count++;
    }
    else{
        (*Q)->rear++;
        (*Q)->count++;
    }
    /*while(i > 0){
            if( (strlen( (*Q)->Array[i].name) < strlen( (*Q)->Array[i-1].name )) || (( strlen((*Q)->Array[i].name) == strlen((*Q)->Array[i-1].name) ) && strcmp( ((*Q)->Array[i].name),((*Q)->Array[i-1].name)) < 0)){
                changedata( ((*Q)->Array), i, (i-1) );
            }
            else{
                return 1;
            }
            i--;
    }*/
    return 0;
}

data_Array_SJFpre * dequeue_Array_SJFpre(qeType_Array_SJFpre ** Q)
{
    data_Array_SJFpre * temp = malloc(sizeof(data_Array_SJFpre));
    int i=0;
    while( i < ((*Q)->count-1) ){
        changedata( (*Q)->Array,i,(i+1) );
        i++;
    }
    temp->arrivaltime = (*Q)->Array[i].arrivaltime;
    temp->bursttime = (*Q)->Array[i].bursttime;
    temp->waitingtime = (*Q)->Array[i].waitingtime;
    strcpy( (temp->name),((*Q)->Array[i].name) );
    (*Q)->rear--;
    (*Q)->count--;
    return temp;
}


void checkT_Array_SJFpre(qeType_Array_SJFpre ** jobQueue, qeType_Array_SJFpre ** readyQueue)          // check arrival time
{
    data_Array_SJFpre * temp = malloc(sizeof(data_Array_SJFpre));
    int i=0;
    if( (*jobQueue)->count != 0 ){
        while( (*jobQueue)->count != 0 && (*jobQueue)->Array[i].arrivaltime == 0){
            temp = dequeue_Array_SJFpre(jobQueue);
            if((*readyQueue)->count == 0){
                (*readyQueue)->Array[(*readyQueue)->rear].arrivaltime = temp->arrivaltime;
                (*readyQueue)->Array[(*readyQueue)->rear].bursttime = temp->bursttime;
                (*readyQueue)->Array[(*readyQueue)->rear].waitingtime = temp->waitingtime;
                strcpy( (*readyQueue)->Array[(*readyQueue)->rear].name , temp->name );
            }
            else{
                (*readyQueue)->Array[(*readyQueue)->rear+1].arrivaltime = temp->arrivaltime;
                (*readyQueue)->Array[(*readyQueue)->rear+1].bursttime = temp->bursttime;
                (*readyQueue)->Array[(*readyQueue)->rear+1].waitingtime = temp->waitingtime;
                strcpy( (*readyQueue)->Array[(*readyQueue)->rear+1].name , temp->name );
            }
            free(temp);
            enqueue_sortburst_SJFpre(readyQueue);
        }
    }

    return;
}

void waitT_Array_SJFpre(qeType_Array_SJFpre ** jobQueue, qeType_Array_SJFpre ** readyQueue)          // arrival time - 1, and check arrival time
{
    int i=0;
    while( i < (*jobQueue)->count){
        (*jobQueue)->Array[i].arrivaltime = (*jobQueue)->Array[i].arrivaltime - 1;
        i++;
    }
    checkT_Array_SJFpre(jobQueue, readyQueue);
    return;
}

void workQ_Array_SJFpre(qeType_Array_SJFpre ** readyQueue, qeType_Array_SJFpre ** recordQueue)     // first data's burst time - 1, if burst time = 0, to recordQueue, others waiting time + 1
{
    data_Array_SJFpre * temp = malloc(sizeof(data_Array_SJFpre));
    int i = 0, check=0;
    while( i < (*readyQueue)->count){
        if(i == 0 && check == 0){
            (*readyQueue)->Array[i].bursttime = (*readyQueue)->Array[i].bursttime - 1;
            check = 1;
            if( ((*readyQueue)->Array[i].bursttime) == 0){
                i = 0;
                temp = dequeue_Array_SJFpre(readyQueue);
                if((*recordQueue)->count == 0){
                    (*recordQueue)->Array[(*recordQueue)->rear].arrivaltime = temp->arrivaltime;
                    (*recordQueue)->Array[(*recordQueue)->rear].bursttime = temp->bursttime;
                    (*recordQueue)->Array[(*recordQueue)->rear].waitingtime = temp->waitingtime;
                    strcpy( (*recordQueue)->Array[(*recordQueue)->rear].name , temp->name );
                }
                else{
                    (*recordQueue)->Array[(*recordQueue)->rear+1].arrivaltime = temp->arrivaltime;
                    (*recordQueue)->Array[(*recordQueue)->rear+1].bursttime = temp->bursttime;
                    (*recordQueue)->Array[(*recordQueue)->rear+1].waitingtime = temp->waitingtime;
                    strcpy( (*recordQueue)->Array[(*recordQueue)->rear+1].name , temp->name );
                }
                enqueue_nonsort_Array_SJFpre(recordQueue);
            }
            else{
                i++;
            }
            free(temp);
        }
        else{
            (*readyQueue)->Array[i].waitingtime = (*readyQueue)->Array[i].waitingtime + 1;
            i++;
        }
    }
    return;
}

void changedata(data_Array_SJFpre Array[],int i,int j)                          // change Array data
{
    int temp;
    char name[10];
    temp = Array[i].arrivaltime;
    Array[i].arrivaltime = Array[j].arrivaltime;
    Array[j].arrivaltime = temp;

    temp = Array[i].bursttime;
    Array[i].bursttime = Array[j].bursttime;
    Array[j].bursttime = temp;

    temp = Array[i].waitingtime;
    Array[i].waitingtime = Array[j].waitingtime;
    Array[j].waitingtime = temp;

    strcpy(name,(Array[i].name));
    strcpy( (Array[i].name), (Array[j].name) );
    strcpy( (Array[j].name), name);

    return;
}
