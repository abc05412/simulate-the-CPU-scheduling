#include "teamproj.h"

data_RR_Array newdata(void);
void sort_arrival(data_RR_Array*);
void checkjob(data_RR_Array*,data_RR_Array*);
void checkready(data_RR_Array*,data_RR_Array*,data_RR_Array*);
void work(data_RR_Array*,data_RR_Array*,data_RR_Array*);
void wait(data_RR_Array*,data_RR_Array*,data_RR_Array*);


void Round_Robin_Array(FILE * fp)
{
    data_RR_Array job,ready,record; //設定三個工作的儲存空間
    job = newdata();    // job一開始儲存資料
    ready = newdata();  // 當job裡面資料的arrivaltime等於零，轉道ready裡面，準備到CPU裡面執行
    record = newdata(); //把ready裡面執行完的程式轉移到record裡面

    char name[20];
    int arrivaltime, bursttime,i=0;

    //下面把資料儲存到job的array
    while(fscanf(fp,"%s %d %d" ,name ,&arrivaltime ,&bursttime) != EOF){
        strcpy(job.name[job.count],name);
        job.arrivaltime[job.count] = arrivaltime;
        job.bursttime[job.count] = bursttime;
        job.waitingtime[job.count] = 0;
        job.checktime[job.count] = 0;
        job.count++;
        job.rear++;

    }

    sort_arrival(&job); //把job裡面依照arrival的優先順續排序(由時間最少小到最多排序)


    checkjob( (&job), (&ready));    //先檢查一次，是否有資料一進來時已經可以準備進CPU執行了

    //下面這個迴圈是模擬在CPU裡面執行的狀況
    //job和ready裡面還有資料沒有處理完時，就會繼續執行
    while( (job.count != 0) || (ready.count != 0) )
    {

        if( (ready.count) != 0)
        {
            work(&job, &ready, &record );    //呼叫work副函式，模擬CPU每執行一個時間單位，檔案裡面的burst時間就會減少1
        }

        wait(&job,&ready,&record);  //呼叫wait副函式，模擬CPU在執行時，待在job裡資料的arrival時間也會跟著減少
    }

    printf("----------\n");
    int k=0;
    double average=0;
    while(k<record.count)     //印出每個資料所需要的等待時間
    {
        average = average+record.waitingtime[k];
        printf("%s %d\n",record.name[k],record.waitingtime[k]);
        k++;
    }
    printf("Average waiting time: %lf",average/record.count);     //印出平均一個資料所需的等待時間
    return ;
}

data_RR_Array newdata(void)  //newdata: 設定一開始空間的起始值
{
    data_RR_Array  Q;
    Q.count = 0;
    Q.front = 0;
    Q.rear = -1;
    return Q;
}

void sort_arrival(data_RR_Array * job)  //sort_arrival: 把job裡面依照arrival的優先順續排序(由時間最少小到最多排序)
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

void checkjob(data_RR_Array * job, data_RR_Array * ready) //checkjob: 確認資料的arrival time是否到了，當等於零時，資料會轉到ready裡面，等待進入CPU
{
    int i,key1,key2;
    char key[20];
    /*{設定job裡資料的arrival time等於零時轉到ready} 是因為我的程式不是先設定特定的時間值去判斷每個資料的arrival time是否等於現在的時間(例如現在的時間為10，所以只要arrival time等於10的都會轉到ready裡面)，
    而是透過CPU每執行一個時間單位，同時把arrivaltime減一個時間單位，所以才會把arrival time為零時將資料轉到ready的資料空間裡面*/
    while(job->arrivaltime[0]==0&&job->count>1)
    {
            ready->rear++;  //第 一筆資料轉給ready
            ready->count++;
            strcpy(ready->name[ready->rear],job->name[0]);
            ready->bursttime[ready->rear] = job->bursttime[0];
            ready->waitingtime[ready->rear] = job->waitingtime[0];
            ready->checktime[ready->rear] = job->waitingtime[0];


            for(i=0;i<job->count-1;i++)  //透過不斷交換，讓後面的每一個值往前移
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
            job->count--;  //job裡面的count減少
    }
    if(job->count==1 && job->arrivaltime[0]==0)  //當最後一筆資料 arrival time為0，資料將直接轉移到ready
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

void work(data_RR_Array* job,data_RR_Array* ready, data_RR_Array* record)  //work: 模擬CPU每執行一個時間單位，檔案裡面的burst時間就會減少1
{

     int check = 0;
     int i=0;
     int k,num[3];
     char key[20];
     while(i<ready->count)
     {

        if(check == 0){  //判斷是否為ready中的第一筆資料
            check = 1;
            ready->bursttime[i] = ready->bursttime[i] - 1;
            ready->checktime[i] = ready->checktime[i] + 1;
            if( (ready->bursttime[i]) == 0){   //如果bursttime歸零時，會被轉移到record裡面，表示資料已經處理完
                checkready(job,ready,record);
                i=0;
            }
            else{  //第一筆資的bursttime減一，checktime時間加一
                i++;
            }
        }
        else{  //其餘資料的waitingtime都加一
            ready->waitingtime[i]++;
            i++;
        }
    }


    if((ready->checktime[0] == robintime )&& (ready->bursttime[0] !=0))  //假如checktime等於robintime(一次最多能做的時間)，則必須移到ready的最後面，等待其他資料處理
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

void checkready(data_RR_Array* job, data_RR_Array* ready, data_RR_Array* record)  //check: 當ready裡面資料的bursttime等於零的時候，轉移到record
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

            for(i=0;i<ready->count-1;i++)  //第一筆資被移走後，其餘資料往前移
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

    if((ready->count ==1 )&& ready->bursttime[0]==0 )  //最後一筆資料完成時，直接轉移到record
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

void wait(data_RR_Array* job, data_RR_Array* ready,data_RR_Array* record)  //wait: 讓在job裡面資料的arrivaltime時間減一
{
    int i=0;
    while(i < job->count&&job->count>0)
    {
        job->arrivaltime[i]--;
        i++;
    }

    checkjob(job, ready);  //確認arrivaltime是否等於零
    return;
}
