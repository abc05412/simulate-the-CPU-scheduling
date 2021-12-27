#include "teamproj.h"

#include "FCFS.c"
#include "RR.c"
#include "SJF_nonpreemptive.c"
#include "SJF_preemptive.c"

int main()
{
    int choose;
    int arrival_time, burst_time, i = 0;
    char filename[20];
    FILE * fp;
    printf("************************************************************************\n");
    printf("*                                                                      *\n");
    printf("* Please input ( 1 ~ 2 ) to choose the way you want to input data:     *\n");
    printf("* 1. user input file name                                              *\n");
    printf("* 2. random data                                                       *\n");
    printf("*                                                                      *\n");
    printf("************************************************************************\n");
    printf("Please enter your choice\n");
    scanf("%d" ,&choose);
    switch(choose){
        case 1:

            printf("Please input a filename:");
            scanf("%s" ,filename);
            break;

        case 2:

            fp = fopen("randomdata.txt","w");
            srand( time(NULL) );
            while(i < 20){
                arrival_time = rand() %20+1;
                burst_time = rand() %10+1;
                fprintf(fp,"P%d\t%d\t%d\n" ,(i+1) ,arrival_time ,burst_time);
                i++;
            }
            fclose(fp);
            strcpy(filename , "randomdata.txt");
            break;

        default:

            printf("Error");
            break;
    }
    if( (choose == 1) || (choose == 2) ){
        printf("\n\n************************************************************************\n");
        printf("* Please input ( 1 ~ 4 ) to choose what type do you want to run:       *\n");
        printf("* 1. First Come First Serve                                            *\n");
        printf("* 2. Round Robin:                                                      *\n");
        printf("* 3. SJF nonpre                                                        *\n");
        printf("* 4. SJF pre                                                           *\n");
        printf("* Input other number to quit                                           *\n");
        printf("************************************************************************\n");
        printf("Please enter your choice\n");
        scanf("%d" ,&choose);
        while( (choose == 1) || (choose == 2) || (choose == 3) || (choose == 4) ){

            fp = fopen(filename,"r");
            switch(choose){
                case 1:

                    printf("FCFS\n");
                    first_come_first_deal(fp);
                    break;

                case 2:

                    printf("RR\n");
                    Round_Robin(fp);
                    break;

                case 3:

                    printf("SJF nonpreemptive\n");
                    SJF_nonpreemptive(fp);
                    break;

                case 4:

                    printf("SJF preemptive\n");
                    SJF_preemptive(fp);
                    break;

                default :

                    printf("Error");
                    break;
            }
            fclose(fp);
            printf("\n\n************************************************************************\n");
            printf("* Please input ( 1 ~ 4 ) to choose what type do you want to run:       *\n");
            printf("* 1. First Come First Serve                                            *\n");
            printf("* 2. Round Robin:                                                      *\n");
            printf("* 3. SJF nonpre                                                        *\n");
            printf("* 4. SJF pre                                                           *\n");
            printf("* Input other number to quit                                           *\n");
            printf("************************************************************************\n");
            printf("Please enter your choice\n");
            scanf("%d" ,&choose);
        }
    }

    return 0;
}
