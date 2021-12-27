#include "teamproj.h"

void add_node_sort(node**,int,int,char*,int);
void printnode(node*);
void wait_time_plus(node**);
void print_wait_time(node*);

void FCFS_Linked_List(FILE * fp)
{
    int arrival_time,cpu_time,burst_time;
    char name[20];
    //initialization
    node * jobqueue = NULL;
    node * readyqueue = NULL;
    node * CPU=NULL;
    node * record=NULL;
    while(fscanf(fp,"%s %d %d",name,&arrival_time,&burst_time)!=EOF)
    {
        add_node_sort(&jobqueue,arrival_time,burst_time,name,0);
    }
    int time = 0;
    while(jobqueue!=NULL||readyqueue!=NULL||CPU!=NULL) // run the time until job ready CPU queue is empty
    {
        printf("now_time:%d\n",time);

        while((jobqueue!=NULL) && (jobqueue->arrival_time==time)) //job queue is not empty and time euqals arrival time move data from job to ready
        {
            add_node_sort(&readyqueue,jobqueue->arrival_time,jobqueue->burst_time,jobqueue->name,jobqueue->cpu_time);
            jobqueue = jobqueue->next;

        }
        if(CPU!=NULL) //check if CPU is empty
        {
            if((CPU->cpu_time + CPU->burst_time) ==time) //check if the process is end if Yes move the data in CPU to record queue
            {
                add_node_sort(&record,CPU->arrival_time,CPU->burst_time,CPU->name,CPU->cpu_time);
                CPU = CPU->next;
            }

        }
        if(readyqueue!=NULL&&CPU==NULL) // if CPU is empty and ready queue is not empty move one data in ready queue to CPU queue
        {
            add_node_sort(&CPU,readyqueue->arrival_time,readyqueue->burst_time,readyqueue->name,time);
            readyqueue = readyqueue->next;
        }

        printf("jobqueue:\n");
        printnode(jobqueue);
        printf("readyqueue:\n");
        printnode(readyqueue);
        printf("cpu:\n");
        printnode(CPU);
        printf("record:\n");
        printnode(record);
        time++;

    }
    printf("wait time:\n");
    print_wait_time(record);
}
void add_node_sort(node** head_address,int arrival_time,int burst_time,char* name,int cpu_time)
{
    node * new_node;
    new_node = malloc(sizeof(node));
    new_node->next = NULL;
    new_node->arrival_time = arrival_time;
    new_node->burst_time = burst_time;
    new_node->cpu_time = cpu_time;
    strcpy(new_node->name,name);
    if((*head_address)==NULL)
    {
        (*head_address) = new_node;
        return;
    }
    else
    {
        node * new_head_address = (*head_address);
        node * pre_head_address = new_head_address;
        if(((*head_address)->arrival_time) > (new_node->arrival_time))
        {
            new_node->next = (*head_address);
            (*head_address) = new_node;
            return;
        }
        else
        {

            while(new_head_address!=NULL)
            {
                if((new_head_address->arrival_time) > (new_node->arrival_time))
                {
                    new_node->next = new_head_address;
                    pre_head_address->next = new_node;
                    return;
                }
                pre_head_address = new_head_address;
                new_head_address = new_head_address->next;
            }

            pre_head_address->next = new_node;
        }
    }
}
void printnode(node* head)
{
    while(head!=NULL)
    {
        printf("%s %d %d \n",head->name,head->arrival_time,head->burst_time);
        head = head->next;
    }
}
void print_wait_time(node * head)
{
    float count=0;
    float sum=0;
    while(head!=NULL)
    {
        printf("%d\n",head->cpu_time-head->arrival_time);

        count++;
        sum = sum + (head->cpu_time-head->arrival_time);
        head = head->next;
    }
    printf("avreage waiting time = %f",(sum/count));
}
