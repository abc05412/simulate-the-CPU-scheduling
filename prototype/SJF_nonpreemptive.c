#include "teamproj.h"

void printdata(process *);
void addjob(process **, char * ,int ,int );
void addready(process **, char * ,int ,int );


void SJF_nonpreemptive(FILE * fp)
{
    process *jobque=NULL,*readyque=NULL,*cpu=NULL;
    int arri,brust,t=0,count=0;
    double waitingtime=0.0;
    char name[20];
    while(fscanf(fp,"%s %d %d",name,&arri,&brust)!=EOF)
    {
        addjob(&jobque,name,arri,brust);
        count++;
    }
    while(cpu !=NULL || jobque !=NULL || readyque != NULL )
    {
        printf("job:\n");
        printdata(jobque);
        printf(" t= %d\n",t);
        printf(" waiting time %lf\n",waitingtime);

        while(jobque!=NULL && jobque->artime == t && jobque->next!=NULL)
        {

            addready(&readyque,jobque->name,jobque->artime,jobque->brustime);
            jobque = jobque->next;
        }
        if(jobque!=NULL && jobque->artime == t && jobque->next==NULL)
        {

            addready(&readyque,jobque->name,jobque->artime,jobque->brustime);
            jobque=NULL;
        }
        if(cpu==NULL && readyque != NULL)
        {
            cpu = readyque;
            cpu->cputime=t;
            readyque = readyque->next;
        }
        else if(cpu!=NULL && readyque != NULL)
        {
            if(cpu->brustime + cpu->cputime == t)
            {
                waitingtime = waitingtime+cpu->cputime - cpu->artime;
                cpu = readyque;
                cpu->cputime=t;
                readyque = readyque->next;
            }
        }
        else if(cpu!=NULL && readyque==NULL)
        {
            if(cpu->brustime+cpu->cputime==t)
            {
                waitingtime = waitingtime + cpu->cputime - cpu->artime;
                cpu = NULL;
            }
        }
        t++;
    }
    printf(" t= %d\n",t);
    printf(" waiting time %lf\n",waitingtime);
    waitingtime = waitingtime / (double)count;
    printf("average waiting time %lf\n",waitingtime);
}

void addjob(process * * L,char *name, int arri,int brust)
{
	process * N, * T;
	N = malloc(sizeof(process));
	N->artime = arri;
	strcpy(N->name,name);
	N->brustime=brust;
	N->next = NULL;
	if (* L == NULL)
    {
        * L = N;
    }
	else
    {
		T = * L;
		if((T->artime) >= (N->artime)){
            N->next = T;
            *L = N;
            return;
        }
        while (T->next != NULL)
        {

            if((T->next->artime) >= (N->artime))
            {
                N->next = T->next;
                T->next = N;
                return;
            }
            T = T->next;
        }
        T->next = N;
    }
}

void addready(process **L,char *name, int arri,int brust)
{
	process * N, * T;
	N = malloc(sizeof(process));
	N->artime = arri;
	strcpy(N->name,name);
	N->brustime=brust;
	N->next = NULL;
	if (* L == NULL)
    {
        * L = N;
    }
	else
    {
		T = * L;
		if((T->brustime) >= (N->brustime))
        {
            N->next = T;
            *L = N;
            return;
        }
        while (T->next != NULL)
        {

            if((T->next->brustime) >= (N->brustime))
            {
                N->next = T->next;
                T->next = N;
                return;
            }
            T = T->next;
        }
        T->next = N;
    }
}


void printdata(process *t)
{
    while(t!=NULL)
    {
        printf("%s %d %d\n",t->name,t->artime,t->brustime);
        t=t->next;
    }
}
