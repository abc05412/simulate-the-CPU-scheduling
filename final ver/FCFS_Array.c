#include "teamproj.h"

void sort(queue*);
void print_queue(queue);


void FCFS_Array(FILE* input)
{   int time = 0;// to be real time
    int i;
    queue job_queue,ready_queue,cpu_queue,record_queue;
    int count =0;//the number of data
    // initialize the size of queue

    ready_queue.rear=0;
    cpu_queue.rear = 0;
    job_queue.rear=0;
    ready_queue.front=0;
    cpu_queue.front = 0;
    job_queue.front= 0;
    cpu_queue.arraival_time[0] = 0;
    cpu_queue.burst_time[0] = 0;
    cpu_queue.name[0][0] = '\0';
    record_queue.rear = 0;
    record_queue.front=0;
    int arraival_time;
    int burst_time;

    char name[200];
    while(fscanf(input,"%s %d %d",name,&arraival_time,& burst_time)!=EOF)
    {   count ++;
        job_queue.arraival_time[job_queue.rear] = arraival_time;
        job_queue.burst_time[job_queue.rear] = burst_time;
        strcpy(job_queue.name[job_queue.rear],name);
        job_queue.rear++;
    }

    sort(&job_queue);// according to the arrvival time to sort

    print_queue(job_queue);

    // -1 represent there is no data in that queue and if there is no any data in those queue then end the loop
    while(job_queue.front<job_queue.rear||ready_queue.front<ready_queue.rear||cpu_queue.rear==1)
    {   while(job_queue.front<job_queue.rear&&job_queue.arraival_time[job_queue.front]==time)
        {   ready_queue.arraival_time[ready_queue.rear] = job_queue.arraival_time[job_queue.front];
            ready_queue.burst_time[ready_queue.rear] = job_queue.burst_time[job_queue.front];
            strcpy(ready_queue.name[ready_queue.rear],job_queue.name[job_queue.front]);

            ready_queue.rear++;
            job_queue.front++;

        }
        if(cpu_queue.rear==1)   //check if CPU is empty
        {   if((cpu_queue.cpu_time[0] + cpu_queue.burst_time[0])==time) //check if the process is end if Yes move the data in CPU to record queue
            {
            record_queue.arraival_time[record_queue.rear] = cpu_queue.arraival_time[0];
            record_queue.burst_time[record_queue.rear] = cpu_queue.burst_time[0];
            strcpy(record_queue.name[record_queue.rear],cpu_queue.name[0]);
            record_queue.cpu_time[record_queue.rear] = cpu_queue.cpu_time[0];

            record_queue.rear++;
            cpu_queue.rear = 0;

            }
        }
        if(ready_queue.front<ready_queue.rear&&cpu_queue.rear==0)   // if CPU is empty and ready queue is not empty move one data in ready queue to CPU queue
        {   cpu_queue.arraival_time[0] = ready_queue.arraival_time[ready_queue.front];
            cpu_queue.burst_time[0] = ready_queue.burst_time[ready_queue.front];
            strcpy(cpu_queue.name[0] ,ready_queue.name[ready_queue.front]);
            cpu_queue.cpu_time[0] = time;
            ready_queue.front++;
            cpu_queue.rear=1;

        }

        printf("%d :\n",time);
       printf("job queue:\n");
        print_queue(job_queue);
        printf("\n");
        printf("ready queue:\n");
        print_queue(ready_queue);
        printf("\n");
        printf("cpu queue:\n");
        print_queue(cpu_queue);
        printf("\n");
        printf("record queue:\n");
        print_queue(record_queue);


        time++;
    }


    float waiting_time;
    int sum=0;
    for(i=0;i<record_queue.rear;i++)
    {
        sum = sum +record_queue.cpu_time[i] - record_queue.arraival_time[i];
    }
        waiting_time = sum/(float)count;
        printf("average vwaiting time:%f\n",waiting_time);

}
void sort(queue*unorder_queue)
{
    int i,j,temp[2];
    char temp_name[5];
    for(j=0;j<unorder_queue->rear;j++)
    {
        for(i=0;i<unorder_queue->rear-j-1;i++)
        {
            if(unorder_queue->arraival_time[i]>unorder_queue->arraival_time[i+1])
            {
                temp[0] = unorder_queue->arraival_time[i];
                temp[1] = unorder_queue->burst_time[i];
                strcpy(temp_name,unorder_queue->name[i]);
                unorder_queue->arraival_time[i] = unorder_queue->arraival_time[i+1];
                unorder_queue->burst_time[i] = unorder_queue->burst_time[i+1];
                strcpy(unorder_queue->name[i],unorder_queue->name[i+1]);
                unorder_queue->arraival_time[i+1] = temp[0];
                unorder_queue->burst_time[i+1] =temp[1];
                strcpy(unorder_queue->name[i+1],temp_name);
            }
        }
    }

}
void print_queue(queue  enter_queue)
{
    int i;

    for(i=enter_queue.front;i<enter_queue.rear;i++)
    {
        printf("%s %d %d\n",enter_queue.name[i],enter_queue.arraival_time[i],enter_queue.burst_time[i]);
    }
}

