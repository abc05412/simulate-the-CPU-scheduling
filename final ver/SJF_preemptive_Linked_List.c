#include "teamproj.h"


void readdata(qeType **, FILE *);
qeType * newqueue(void);
int enqueue_sortarrival(qeType ** , data *);
int enqueue_sortburst(qeType ** , data *);
int enqueue_nonsort(qeType ** , data *);
data * dequeue(qeType ** );
data * frontdata(qeType * );
data * reardata(qeType * );
int delqueue(qeType ** );
void checkT(qeType ** , qeType **);
void waitT(qeType ** , qeType **);
void workQ(qeType ** , qeType **);

int SJF_preemptive_Linked_List(FILE * fp)
{
    data * current;

    qeType * jobQ, * readyQ, *recordQ;
    jobQ = newqueue();
    readyQ = newqueue();
    recordQ = newqueue();
    readdata(&jobQ,fp);                                   // read data to jobQ, and sort it by arrival time
    checkT( (&jobQ), (&readyQ));                          // check jobQ's arrival time = 0 , if = 0 to readyQ, and sort it by burst time
    while( (jobQ->count != 0) || (readyQ->count != 0) ){
        if( (readyQ->count) != 0){
            workQ( (&readyQ), (&recordQ) );               // readyQ's first burst time - 1, if burst time = 0 to recordQ, and sort it by name, others waiting time + 1
        }
        waitT( (&jobQ), (&readyQ));                       // jobQ's arrival time - 1, if arrival time = 0, data to readyQ, and sort it by burst time
    }

    double sum=0.0;
    current = recordQ->front;
    printf("----------\n");
    while(current != NULL){
        printf("%s  %d\n" ,current->name, current->waitingtime);
        sum = sum + current->waitingtime;
        current = current->next;
    }
    printf("----------\n\n");
    printf("Average waiting time: %f\n\n\n",(sum/(recordQ->count)) );

    return 0;
}

void readdata(qeType ** Q, FILE * fp)                   // read data to jobQ, and sort it by arrival time
{
    char name[20];
    int arrivaltime, bursttime;
    data * newdata;
    while(fscanf(fp,"%s %d %d" ,name ,&arrivaltime ,&bursttime) != EOF){
        newdata = malloc(sizeof(data));
        strcpy(newdata->name,name);
        newdata->arrivaltime = arrivaltime;
        newdata->bursttime = bursttime;
        newdata->waitingtime = 0;
        newdata->next = NULL;
        enqueue_sortarrival(Q, newdata);
    }
    return;
}

qeType * newqueue(void)                                 // create a new queue
{
    qeType * Q;
    Q = malloc(sizeof(qeType));
    Q->count = 0;
    Q->front = NULL;
    Q->rear = NULL;
    return Q;
}

int enqueue_sortarrival(qeType ** Q, data * newdata)        // sort data by arrival time
{
    data * current, * previous;
    (*Q)->count++;
    if( ((*Q)->front == NULL) && ((*Q)->rear == NULL) ){
        (*Q)->front = newdata;
        (*Q)->rear = newdata;
    }
    else{
        current = (*Q)->front;
        previous = current;
        while(current != NULL ){
            if(current->arrivaltime > newdata->arrivaltime){
                if(previous == current){
                    newdata->next = (*Q)->front;
                    (*Q)->front = newdata;
                }
                else{
                    newdata->next = current;
                    previous->next = newdata;
                }
                return 1;
            }
            previous = current;
            current = current->next;
        }
        if(current == NULL){
            (*Q)->rear->next = newdata;
            (*Q)->rear = newdata;
            return 1;
        }
    }
    return 0;
}

int enqueue_sortburst(qeType ** Q, data * newdata)        // sort data by burst time
{
    data * current, * previous;
    (*Q)->count++;
    if( ((*Q)->front == NULL) && ((*Q)->rear == NULL) ){
        (*Q)->front = newdata;
        (*Q)->rear = newdata;
    }
    else{
        current = (*Q)->front;
        previous = current;
        while(current != NULL ){
            if(current->bursttime > newdata->bursttime){
                if(previous == current){
                    newdata->next = current;
                    (*Q)->front = newdata;
                }
                else{
                    newdata->next = current;
                    previous->next = newdata;
                }
                return 1;
            }
            previous = current;
            current = current->next;
        }
        (*Q)->rear->next = newdata;
        (*Q)->rear = newdata;
        return 1;

    }
    return 0;
}

int enqueue_nonsort(qeType ** Q, data * newdata)       // sort data by name
{
    data * current, * previous;
    (*Q)->count++;
    if( ((*Q)->front == NULL) && ((*Q)->rear == NULL) ){
        (*Q)->front = newdata;
        (*Q)->rear = newdata;
    }
    else{
        (*Q)->rear->next = newdata;
        (*Q)->rear = newdata;
        return 1;
    }
    return 0;
}

data * dequeue(qeType ** Q)
{
    data * head;
    (*Q)->count--;
    head = frontdata( (*Q) );
    (*Q)->front = (*Q)->front->next;
    if( (*Q)->front == NULL ){
        (*Q)->rear = NULL;
    }
    head->next = NULL;
    return head;
}

data * frontdata(qeType * Q)
{
    return (Q->front);
}

data * reardata(qeType * Q)
{
    return (Q->rear);
}

int delqueue(qeType ** Q)
{
    data * current, * previous;
    current = frontdata( (*Q) );
    previous = current;
    while(current != NULL){
        previous = current;
        current = current->next;
        free(previous);
    }
    return 1;
}

void checkT(qeType ** jobQueue, qeType ** readyQueue)          // check arrival time
{
    data * current;
    current = (*jobQueue)->front;
    if(current != NULL){
        while(current != NULL && current->arrivaltime == 0){
            current = current->next;
            enqueue_sortburst(readyQueue, dequeue(jobQueue) );
        }
    }
    return;
}

void waitT(qeType ** jobQueue, qeType ** readyQueue)          // arrival time - 1, and check arrival time
{
    data * current;
    current = (*jobQueue)->front;
    while(current != NULL){
        current->arrivaltime = current->arrivaltime - 1;
        current = current->next;
    }
    checkT(jobQueue, readyQueue);
    return;
}

void workQ(qeType ** readyQueue, qeType ** recordQueue)     // first data's burst time - 1, if burst time = 0, to recordQueue, others waiting time + 1
{
    data * current;
    int check = 0;
    current = (*readyQueue)->front;
    while(current != NULL){
        if(check == 0){
            check = 1;
            current->bursttime = current->bursttime - 1;
            if( (current->bursttime) == 0){
                current = current->next;
                enqueue_nonsort(recordQueue, dequeue(readyQueue) );
            }
            else{
                current = current->next;
            }
        }
        else{
            current->waitingtime = current->waitingtime + 1;
            current = current->next;
        }
    }
    return;
}
