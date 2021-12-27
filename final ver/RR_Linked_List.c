#include "teamproj.h"

void readdata_2(qeType_2 **, FILE *);
qeType_2 * newqueue_2(void);
int enqueue_sortarrival_2(qeType_2 ** , data_2 *);
int enqueue_sortname_2(qeType_2 ** , data_2 *);
data_2 * dequeue_2(qeType_2 ** );
data_2 * frontdata_2(qeType_2 * );
data_2 * reardata_2(qeType_2 * );
void checkT_2(qeType_2 ** , qeType_2 **);
void waitT_2(qeType_2 ** , qeType_2 **);
void workQ_2(qeType_2 ** , qeType_2 **);

void Round_Robin_Linked_List(FILE * fp)
{
    data_2 * current;

    qeType_2 * jobQ, * readyQ, *recordQ;
    jobQ = newqueue_2();
    readyQ = newqueue_2();
    recordQ = newqueue_2();
    readdata_2(&jobQ,fp);           // read data to job queue
    checkT_2( (&jobQ), (&readyQ));  // check job queue's arrival time
    while( (jobQ->count != 0) || (readyQ->count != 0) ){
        if( (readyQ->count) != 0){
            workQ_2( (&readyQ), (&recordQ) );       // ready queue's first data's burst time - 1,and check it = 0 or not, if it = 0 data move to record queue
        }                                           // other's waiting time + 1
        waitT_2( (&jobQ), (&readyQ));               // job queue's arrival time - 1, and check it = 0 or not, if it = 0. data move to ready queue
    }

    double sum = 0.0;
    current = recordQ->front;
    printf("----------\n");
    while(current != NULL){
        printf("%s  %d\n" ,current->name, current->waitingtime);
        sum = sum + current->waitingtime;
        current = current->next;
    }
    printf("----------\n\n");
    printf("Average waiting time: %f\n\n\n",(sum/recordQ->count) );

    return;
}

void readdata_2(qeType_2 ** Q, FILE * fp)       // read data to a queue
{
    char name[20];
    int arrivaltime, bursttime;
    data_2 * newdata;
    while(fscanf(fp,"%s %d %d" ,name ,&arrivaltime ,&bursttime) != EOF){
        newdata = malloc(sizeof(data_2));
        strcpy(newdata->name,name);
        newdata->arrivaltime = arrivaltime;
        newdata->bursttime = bursttime;
        newdata->waitingtime = 0;
        newdata->checktime = 0;
        newdata->next = NULL;
        enqueue_sortarrival_2(Q, newdata);
    }
    return;
}

qeType_2 * newqueue_2(void)             // create a new queue
{
    qeType_2 * Q;
    Q = malloc(sizeof(qeType_2));
    Q->count = 0;
    Q->front = NULL;
    Q->rear = NULL;
    return Q;
}

int enqueue_sortarrival_2(qeType_2 ** Q, data_2 * newdata)      // input a data to queue, and sort it by arrivaltime
{
    data_2 * current, * previous;
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

int enqueue_sortname_2(qeType_2 ** Q, data_2 * newdata)     // input a data to queue, and sort it by name
{
    data_2 * current, * previous;
    (*Q)->count++;
    if( ((*Q)->front == NULL) && ((*Q)->rear == NULL) ){
        (*Q)->front = newdata;
        (*Q)->rear = newdata;
    }
    else{
        current = (*Q)->front;
        previous = current;
        while(current != NULL ){
            if( (strlen(current->name) > strlen(newdata->name) ) || ((strlen(current->name) == strlen(newdata->name) ) && (strcmp((current->name),(newdata->name)) > 0)) ){
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
        (*Q)->rear->next = newdata;
        (*Q)->rear = newdata;
        return 1;
    }
    return 0;
}

data_2 * dequeue_2(qeType_2 ** Q)       // remove the first data, and return its value
{
    data_2 * head;
    (*Q)->count--;
    head = frontdata_2( (*Q) );
    (*Q)->front = (*Q)->front->next;
    if( (*Q)->front == NULL ){
        (*Q)->rear = NULL;
    }
    head->next = NULL;
    return head;
}

data_2 * frontdata_2(qeType_2 * Q)
{
    return (Q->front);
}

data_2 * reardata_2(qeType_2 * Q)
{
    return (Q->rear);
}

void checkT_2(qeType_2 ** jobQueue, qeType_2 ** readyQueue)     // check job queue's arrival time, if it = 0, data move to ready queue
{
    data_2 * current;
    current = (*jobQueue)->front;
    if(current != NULL){
        while(current != NULL && current->arrivaltime == 0){
            current = current->next;
            enqueue_sortarrival_2(readyQueue, dequeue_2(jobQueue) );
        }
    }
    return;
}

void waitT_2(qeType_2 ** jobQueue, qeType_2 ** readyQueue)      // job queue's arrival time - 1, and check it = 0 or not, if it = 0. data move to ready queue
{
    data_2 * current;
    current = (*jobQueue)->front;
    while(current != NULL){
        current->arrivaltime = current->arrivaltime - 1;
        current = current->next;
    }
    checkT_2(jobQueue, readyQueue);
    return;
}

void workQ_2(qeType_2 ** readyQueue, qeType_2 ** recordQueue)   // ready queue's first data's burst time - 1,and check it = 0 or not, if it = 0 data move to record queue
{                                                               // other's waiting time + 1
    data_2 * current,*first;
    int check = 0;
    first = (*readyQueue)->front;
    current = (*readyQueue)->front;
    while(current != NULL){
        if(check == 0){
            check = 1;
            current->bursttime = current->bursttime - 1;
            current->checktime = current->checktime + 1;
            if( (current->bursttime) == 0){
                current = current->next;
                enqueue_sortname_2(recordQueue, dequeue_2(readyQueue) );
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

    if((first->checktime == robintime )&& first->bursttime !=0)
    {
        first->checktime = 0;
        enqueue_sortarrival_2(readyQueue,dequeue_2(readyQueue));
    }
    return;
}
