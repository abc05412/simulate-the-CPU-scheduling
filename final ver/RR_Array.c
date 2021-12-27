#include "teamproj.h"

data_RR_Array newdata(void);
void sort_arrival(data_RR_Array*);
void checkjob(data_RR_Array*,data_RR_Array*);
void checkready(data_RR_Array*,data_RR_Array*,data_RR_Array*);
void work(data_RR_Array*,data_RR_Array*,data_RR_Array*);
void wait(data_RR_Array*,data_RR_Array*,data_RR_Array*);


void Round_Robin_Array(FILE * fp)
{
    data_RR_Array job,ready,record; //�]�w�T�Ӥu�@���x�s�Ŷ�
    job = newdata();    // job�@�}�l�x�s���
    ready = newdata();  // ��job�̭���ƪ�arrivaltime����s�A��Dready�̭��A�ǳƨ�CPU�̭�����
    record = newdata(); //��ready�̭����槹���{���ಾ��record�̭�

    char name[20];
    int arrivaltime, bursttime,i=0;

    //�U�������x�s��job��array
    while(fscanf(fp,"%s %d %d" ,name ,&arrivaltime ,&bursttime) != EOF){
        strcpy(job.name[job.count],name);
        job.arrivaltime[job.count] = arrivaltime;
        job.bursttime[job.count] = bursttime;
        job.waitingtime[job.count] = 0;
        job.checktime[job.count] = 0;
        job.count++;
        job.rear++;

    }

    sort_arrival(&job); //��job�̭��̷�arrival���u������Ƨ�(�Ѯɶ��̤֤p��̦h�Ƨ�)


    checkjob( (&job), (&ready));    //���ˬd�@���A�O�_����Ƥ@�i�Ӯɤw�g�i�H�ǳƶiCPU����F

    //�U���o�Ӱj��O�����bCPU�̭����檺���p
    //job�Mready�̭��٦���ƨS���B�z���ɡA�N�|�~�����
    while( (job.count != 0) || (ready.count != 0) )
    {

        if( (ready.count) != 0)
        {
            work(&job, &ready, &record );    //�I�swork�ƨ禡�A����CPU�C����@�Ӯɶ����A�ɮ׸̭���burst�ɶ��N�|���1
        }

        wait(&job,&ready,&record);  //�I�swait�ƨ禡�A����CPU�b����ɡA�ݦbjob�̸�ƪ�arrival�ɶ��]�|��۴��
    }

    printf("----------\n");
    int k=0;
    double average=0;
    while(k<record.count)     //�L�X�C�Ӹ�Ʃһݭn�����ݮɶ�
    {
        average = average+record.waitingtime[k];
        printf("%s %d\n",record.name[k],record.waitingtime[k]);
        k++;
    }
    printf("Average waiting time: %lf",average/record.count);     //�L�X�����@�Ӹ�Ʃһݪ����ݮɶ�
    return ;
}

data_RR_Array newdata(void)  //newdata: �]�w�@�}�l�Ŷ����_�l��
{
    data_RR_Array  Q;
    Q.count = 0;
    Q.front = 0;
    Q.rear = -1;
    return Q;
}

void sort_arrival(data_RR_Array * job)  //sort_arrival: ��job�̭��̷�arrival���u������Ƨ�(�Ѯɶ��̤֤p��̦h�Ƨ�)
{
    int i,k,key1,key2;
    char key[20];
    for(k=0;k<job->count;k++)
    {
        for(i=0;i<job->count-1;i++)
        {
            if(job->arrivaltime[i] > job->arrivaltime[i+1])
            {
                strcpy(key,job->name[i]);
                key1 = job->arrivaltime[i];
                key2 = job->bursttime[i];

                strcpy(job->name[i],job->name[i+1]);
                job->arrivaltime[i] = job->arrivaltime[i+1];
                job->bursttime[i] = job->bursttime[i+1];

                strcpy(job->name[i+1],key);
                job->arrivaltime[i+1] = key1;
                job->bursttime[i+1] = key2;
            }
        }
    }
    return ;
}

void checkjob(data_RR_Array * job, data_RR_Array * ready) //checkjob: �T�{��ƪ�arrival time�O�_��F�A����s�ɡA��Ʒ|���ready�̭��A���ݶi�JCPU
{
    int i,key1,key2;
    char key[20];
    /*{�]�wjob�̸�ƪ�arrival time����s�����ready} �O�]���ڪ��{�����O���]�w�S�w���ɶ��ȥh�P�_�C�Ӹ�ƪ�arrival time�O�_����{�b���ɶ�(�Ҧp�{�b���ɶ���10�A�ҥH�u�narrival time����10�����|���ready�̭�)�A
    �ӬO�z�LCPU�C����@�Ӯɶ����A�P�ɧ�arrivaltime��@�Ӯɶ����A�ҥH�~�|��arrival time���s�ɱN������ready����ƪŶ��̭�*/
    while(job->arrivaltime[0]==0&&job->count>1)
    {
            ready->rear++;  //�� �@������൹ready
            ready->count++;
            strcpy(ready->name[ready->rear],job->name[0]);
            ready->bursttime[ready->rear] = job->bursttime[0];
            ready->waitingtime[ready->rear] = job->waitingtime[0];
            ready->checktime[ready->rear] = job->waitingtime[0];


            for(i=0;i<job->count-1;i++)  //�z�L���_�洫�A���᭱���C�@�ӭȩ��e��
            {
                strcpy(key,job->name[i]);
                key1 = job->arrivaltime[i];
                key2 = job->bursttime[i];

                strcpy(job->name[i],job->name[i+1]);
                job->arrivaltime[i] = job->arrivaltime[i+1];
                job->bursttime[i] = job->bursttime[i+1];

                strcpy(job->name[i+1],key);
                job->arrivaltime[i+1] = key1;
                job->bursttime[i+1] = key2;
            }
            job->count--;  //job�̭���count���
    }
    if(job->count==1 && job->arrivaltime[0]==0)  //��̫�@����� arrival time��0�A��ƱN�����ಾ��ready
    {
        ready->rear++;
        ready->count++;
        strcpy(ready->name[ready->rear],job->name[0]);
        ready->bursttime[ready->rear] = job->bursttime[0];
        ready->waitingtime[ready->rear] = job->waitingtime[0];
        ready->checktime[ready->rear] = job->waitingtime[0];
        job->count--;
    }
    return ;
}

void work(data_RR_Array* job,data_RR_Array* ready, data_RR_Array* record)  //work: ����CPU�C����@�Ӯɶ����A�ɮ׸̭���burst�ɶ��N�|���1
{

     int check = 0;
     int i=0;
     int k,num[3];
     char key[20];
     while(i<ready->count)
     {

        if(check == 0){  //�P�_�O�_��ready�����Ĥ@�����
            check = 1;
            ready->bursttime[i] = ready->bursttime[i] - 1;
            ready->checktime[i] = ready->checktime[i] + 1;
            if( (ready->bursttime[i]) == 0){   //�p�Gbursttime�k�s�ɡA�|�Q�ಾ��record�̭��A��ܸ�Ƥw�g�B�z��
                checkready(job,ready,record);
                i=0;
            }
            else{  //�Ĥ@���ꪺbursttime��@�Achecktime�ɶ��[�@
                i++;
            }
        }
        else{  //��l��ƪ�waitingtime���[�@
            ready->waitingtime[i]++;
            i++;
        }
    }


    if((ready->checktime[0] == robintime )&& (ready->bursttime[0] !=0))  //���pchecktime����robintime(�@���̦h�వ���ɶ�)�A�h��������ready���̫᭱�A���ݨ�L��ƳB�z
    {
        for(k=0;k<ready->count-1;k++)
        {
            strcpy(key,ready->name[k]);
            num[0] = ready->bursttime[k];
            num[1] = ready->waitingtime[k];
            num[2] = ready->checktime[k];


            strcpy(ready->name[k],ready->name[k+1]);
            ready->bursttime[k] = ready->bursttime[k+1];
            ready->waitingtime[k] = ready->waitingtime[k+1];
            ready->checktime[k] = ready->checktime[k+1];

            strcpy(ready->name[k+1],key);
            ready->bursttime[k+1] = num[0];
            ready->waitingtime[k+1] = num[1];
            ready->checktime[k+1] = num[2];
        }
        ready->checktime[k] = 0;

    }
    return;
}

void checkready(data_RR_Array* job, data_RR_Array* ready, data_RR_Array* record)  //check: ��ready�̭���ƪ�bursttime����s���ɭԡA�ಾ��record
{
    int i,num[3];
    char key[20];
    if(ready->count>1)
    {
        if(ready->bursttime[0]==0)
        {
            record->rear++;
            record->count++;
            strcpy(record->name[record->rear],ready->name[0]);
            record->bursttime[record->rear] = ready->bursttime[0];
            record->waitingtime[record->rear] = ready->waitingtime[0];
            record->checktime[record->rear] = ready->waitingtime[0];

            for(i=0;i<ready->count-1;i++)  //�Ĥ@����Q������A��l��Ʃ��e��
            {
                strcpy(key,ready->name[i]);
                num[0] = ready->bursttime[i];
                num[1] = ready->waitingtime[i];
                num[2] = ready->checktime[i];


                strcpy(ready->name[i],ready->name[i+1]);
                ready->bursttime[i] = ready->bursttime[i+1];
                ready->waitingtime[i] = ready->waitingtime[i+1];
                ready->checktime[i] = ready->checktime[i+1];

                strcpy(ready->name[i+1],key);
                ready->bursttime[i+1] = num[0];
                ready->waitingtime[i+1] = num[1];
                ready->checktime[i+1] = num[2];
            }
            ready->count--;
            ready->rear--;

        }
    }

    if((ready->count ==1 )&& ready->bursttime[0]==0 )  //�̫�@����Ƨ����ɡA�����ಾ��record
    {
        record->rear++;
        record->count++;
        strcpy(record->name[record->rear],ready->name[0]);
        record->bursttime[record->rear] = ready->bursttime[0];
        record->waitingtime[record->rear] = ready->waitingtime[0];
        record->checktime[record->rear] = ready->waitingtime[0];
        ready->count--;
    }

    return ;
}

void wait(data_RR_Array* job, data_RR_Array* ready,data_RR_Array* record)  //wait: ���bjob�̭���ƪ�arrivaltime�ɶ���@
{
    int i=0;
    while(i < job->count&&job->count>0)
    {
        job->arrivaltime[i]--;
        i++;
    }

    checkjob(job, ready);  //�T�{arrivaltime�O�_����s
    return;
}
