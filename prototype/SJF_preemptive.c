#include "teamproj.h"


void readdata(qeType **, FILE *);
qeType * newqueue(void);
int enqueue_sortarrival(qeType ** , data *);
int enqueue_sortburst(qeType ** , data *);
int enqueue_sortname(qeType ** , data *);
data * dequeue(qeType ** );
data * frontdata(qeType * );
data * reardata(qeType * );
int delqueue(qeType ** );
void checkT(qeType ** , qeType **);
void waitT(qeType ** , qeType **);
void workQ(qeType ** , qeType **);

int SJF_preemptive(FILE * fp)
{
    data * current;

    qeType * jobQ, * readyQ, *recordQ;
    jobQ = newqueue();
    readyQ = newqueue();
    recordQ = newqueue();
    readdata(&jobQ,fp);
    checkT( (&jobQ), (&readyQ));
    while( (jobQ->count != 0) || (readyQ->count != 0) ){
        if( (readyQ->count) != 0){
            workQ( (&readyQ), (&recordQ) );
        }
        waitT( (&jobQ), (&readyQ));
    }

    current = recordQ->front;
    printf("----------\n");
    while(current != NULL){
        printf("%s  %d\n" ,current->name, current->waitingtime);
        current = current->next;
    }
    printf("----------\n\n\n");

    return 0;
}

void readdata(qeType ** Q, FILE * fp)
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

qeType * newqueue(void)
{
    qeType * Q;
    Q = malloc(sizeof(qeType));
    Q->count = 0;
    Q->front = NULL;
    Q->rear = NULL;
    return Q;
}

int enqueue_sortarrival(qeType ** Q, data * newdata)
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

int enqueue_sortburst(qeType ** Q, data * newdata)
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

int enqueue_sortname(qeType ** Q, data * newdata)
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

void checkT(qeType ** jobQueue, qeType ** readyQueue)
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

void waitT(qeType ** jobQueue, qeType ** readyQueue)
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

void workQ(qeType ** readyQueue, qeType ** recordQueue)
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
                enqueue_sortname(recordQueue, dequeue(readyQueue) );
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
