#include "teamproj.h"

#include "FCFS_Linked_List.c"
#include "FCFS_Array.c"
#include "RR_Linked_List.c"
#include "RR_Array.c"
#include "SJF_nonpreemptive_Linked_List.c"
#include "SJF_nonpreemptive_Array.c"
#include "SJF_preemptive_Linked_List.c"
#include "SJF_preemptive_Array.c"

int main()
{
    int choose;
    int arrival_time, burst_time, i = 0;
    char filename[20];
    FILE * fp;
    printf("************************************************************************\n");  /* User choose the way to input data */
    printf("*                                                                      *\n");
    printf("* Please input ( 1 ~ 2 ) to choose the way you want to input data:     *\n");
    printf("* 1. user input file name                                              *\n");
    printf("* 2. random data                                                       *\n");
    printf("*                                                                      *\n");
    printf("************************************************************************\n");
    printf("Please enter your choice\n");
    scanf("%d",&choose);
    switch(choose){
    case 1:

        printf("Please input a filename:");
        scanf("%s",filename);
        break;

    case 2:

        fp = fopen("randomdata.txt","w");
        srand( time(NULL) );
        while(i < 90){
            arrival_time = rand() %10+1;
            burst_time = rand() %30+1;
            fprintf(fp,"P%d\t%d\t%d\n",(i+1),arrival_time,burst_time);
            i++;
        }
        fclose(fp);
        strcpy(filename, "randomdata.txt");
        break;

    default:

        printf("Error");
        break;
    }
    if( (choose == 1) || (choose == 2) ){
        printf("\n\n************************************************************************\n");  /* User choose the type and version to run the data */
        printf("* Please input ( 1 ~ 4 ) to choose what type do you want to run:       *\n");
        printf("* 1. First Come First Serve                                            *\n");
        printf("* 2. Round Robin:                                                      *\n");
        printf("* 3. SJF nonpre                                                        *\n");
        printf("* 4. SJF pre                                                           *\n");
        printf("* Input other number to quit                                           *\n");
        printf("************************************************************************\n");
        printf("Please enter your choice\n");
        scanf("%d",&choose);
        while( (choose == 1) || (choose == 2) || (choose == 3) || (choose == 4) ){

            fp = fopen(filename,"r");
            switch(choose){
            case 1:

                printf("\n\n************************************************************************\n");
                printf("* Please input ( 1 ~ 2 ) to choose which version do you want to run:   *\n");
                printf("* 1. Linked list:                                                      *\n");
                printf("* 2. Array:                                                            *\n");
                printf("************************************************************************\n");
                printf("Please enter your choice\n");
                scanf("%d",&choose);
                switch(choose){
                case 1:
                    printf("FCFS Linked List\n");
                    FCFS_Linked_List(fp);
                    break;
                case 2:
                    printf("FCFS Array\n");
                    FCFS_Array(fp);
                    break;
                default :
                    printf("Error\n");
                    break;
                }
                break;

            case 2:

                printf("\n\n************************************************************************\n");
                printf("* Please input ( 1 ~ 2 ) to choose which version do you want to run:   *\n");
                printf("* 1. Linked list:                                                      *\n");
                printf("* 2. Array:                                                            *\n");
                printf("************************************************************************\n");
                printf("Please enter your choice\n");
                scanf("%d",&choose);
                switch(choose){
                case 1:
                    printf("RR Linked List\n");
                    Round_Robin_Linked_List(fp);
                    break;
                case 2:
                    printf("RR Array\n");
                    Round_Robin_Array(fp);
                    break;
                default :
                    printf("Error\n");
                    break;
                }
                break;

            case 3:

                printf("\n\n************************************************************************\n");
                printf("* Please input ( 1 ~ 2 ) to choose which version do you want to run:   *\n");
                printf("* 1. Linked list:                                                      *\n");
                printf("* 2. Array:                                                            *\n");
                printf("************************************************************************\n");
                printf("Please enter your choice\n");
                scanf("%d",&choose);
                switch(choose){
                case 1:
                    printf("SJF nonpreemptive Linked List\n");
                    SJF_nonpreemptive_Linked_List(fp);
                    break;
                case 2:
                    printf("SJF nonpreemptive Array\n");
                    SJF_nonpreemptive_Array(fp);
                    break;
                default :
                    printf("Error\n");
                    break;
                }
                break;

            case 4:

                printf("\n\n************************************************************************\n");
                printf("* Please input ( 1 ~ 2 ) to choose which version do you want to run:   *\n");
                printf("* 1. Linked list:                                                      *\n");
                printf("* 2. Array:                                                            *\n");
                printf("************************************************************************\n");
                printf("Please enter your choice\n");
                scanf("%d",&choose);
                switch(choose){
                case 1:
                    printf("SJF preemptive Linked List\n");
                    SJF_preemptive_Linked_List(fp);
                    break;
                case 2:
                    printf("SJF preemptive Array\n");
                    SJF_preemptive_Array(fp);
                    break;
                default :
                    printf("Error\n");
                    break;
                }
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
            scanf("%d",&choose);
        }
    }

    return 0;
}
