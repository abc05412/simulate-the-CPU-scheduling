#include "teamproj.h"

void printarray(process_Array *,int);
void addjobarray(process_Array [], char *,int,int,int);
void addreadyarray(process_Array [],  char *,int,int,int);

void SJF_nonpreemptive_Array(FILE * input) //present the SJF non-preemptive using array
{
    process_Array *jobque=NULL,*readyque=NULL,*cpu;
    jobque = malloc(sizeof(process_Array)*100);
    readyque = malloc(sizeof(process_Array)*100);  // array max is 100
    cpu = malloc(sizeof(process_Array));
    cpu = NULL;
    int arri,brust,t=0,count=0,jobcount=0,readcount=0;
    double waitingtime=0.0;
    char name[20];
    while(fscanf(input,"%s %d %d",name,&arri,&brust)!=EOF)
    {
        count++;  // reading file and add data to jobqueue
        addjobarray(jobque,name,arri,brust,count);
    }
    jobcount = count;
    while(cpu !=NULL || jobcount>0 || readcount>0 )
    {
        printf("job:\n");
        //printarray(jobque,jobcount);
        printf(" t= %d\n",t);
        printf(" waiting time %lf\n",waitingtime);
        // let data into readyqueue according to the arrival time
        while(jobcount>1 && jobque->artime == t )
        {
            // count the number of readyqueue and jobqueue
            readcount++;
            addreadyarray(readyque,jobque[0].name,jobque[0].artime,jobque[0].brustime,readcount);
            jobque = jobque+1;
            jobcount--;
        }
        if(jobcount==1 && jobque->artime == t )
        {
            readcount++;
            addreadyarray(readyque,jobque[0].name,jobque[0].artime,jobque[0].brustime,readcount);
            jobcount--;
            jobque=NULL;
        }
        if(cpu==NULL && readcount>0)
        {
            cpu = readyque;  //let data which is smallest brust time go into cup
            cpu->cputime=t;  //record the time into cpu
            readyque = readyque+1;
            readcount--;
        }
        else if(cpu!=NULL && readcount>0)
        {
            if(cpu->brustime + cpu->cputime == t)
            {
                waitingtime = waitingtime+cpu->cputime - cpu->artime;  // counting waiting time
                cpu = readyque;  // let the data into cpu
                cpu->cputime=t;
                readyque = readyque+1;  //change the head of readyqueue
                readcount--;  // count the number of readyqueue
            }
        }
        else if(cpu!=NULL && readcount==0)
        {
            if(cpu->brustime+cpu->cputime==t)
            {
                waitingtime = waitingtime + cpu->cputime - cpu->artime;
                cpu = NULL;  //clear cpu
                //free(readyque);
                readyque = NULL;
            }
        }
        t++;
    }
    printf(" t= %d\n",t);
    printf(" waiting time %lf\n",waitingtime);
    waitingtime = waitingtime / (double)count;
    printf("average waiting time %lf\n",waitingtime);  // calculating average waiting time
}

void addjobarray(process_Array array[],char *name, int arri,int brust,int count)
{
    process_Array p,key;  // sort jobqueue according arrival time using insertion sort
    p.artime = arri;
    p.name = malloc(sizeof(char*));
    p.name = name;
    p.brustime = brust;
    p.cputime = 0;
    int i,j;
    array[count-1] =p;

    for(i=1; i<count; i++)
    {
        key = array[i];
        j=i;
        while(j>0 && key.artime<array[j-1].artime)
        {
            array[j] = array[j-1];
            j--;
        }
        array[j] = key;
    }

}

void addreadyarray(process_Array array[],char *name, int arri,int brust,int count)
{
    process_Array p,key;  // sort jobqueue according brust time using insertion sort
    p.artime = arri;
    p.name = malloc(sizeof(char*));
    p.name = name;
    p.brustime = brust;
    p.cputime = 0;
    int i,j;
    array[count-1] =p;
    for(i=1; i < count; i++)
    {
        key = array[i];
        j=i;
        while(j>0 && key.brustime < array[j-1].brustime)
        {
            array[j] = array[j-1];
            j--;
        }
        array[j] = key;
    }

}


void printarray(process_Array *t,int count)  //print the content of array
{
    for(int i=0; i<count; i++)
    {
        printf("%s %d %d\n",t->name,t->artime,t->brustime);
        t=t+1;
    }
}
