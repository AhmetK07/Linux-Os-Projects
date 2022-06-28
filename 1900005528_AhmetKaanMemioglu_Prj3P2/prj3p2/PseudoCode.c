#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<pthread.h>
#include<sys/wait.h>
#include<sys/stat.h>


void *PseudoTerminal();
void *ResultPrint();

int main()
{
    
    pthread_t Pthread1;
    pthread_t Pthread2;

    if(pthread_create(&Pthread1,NULL,ResultPrint,NULL) != 0)
    {
        perror("Encountered some problems while the process of creating of thread 1!\n");
        return -1;
    }
    if (pthread_create(&Pthread2,NULL,PseudoTerminal,NULL) != 0)
    {
        perror("Encountered some problems while the process of creating of thread 2!\n");
        return -1;
    }
    
    pid_t Parent = getpid();
    pid_t ParentOfParent = getppid();


    printf("User's desired process id of the Parent: [%d] \n",Parent);
    printf("User's created Parent of the Parent process: [%d] \n",ParentOfParent);

    printf("#___________________________________________________#\n");
    
    printf("Thread Id of Parent's First Child: [%u]\n",Pthread1);
    printf("Thread Id of Parent's Second Child: [%u]\n",Pthread2);

    if (pthread_join(Pthread1,NULL) != 0)
    {
        perror("Encountered some problems while creating Threadjoin!\n");
        return -1;
    }

    if (pthread_join(Pthread2,NULL) != 0)
    {
        perror("Encountered some problems while creating Threadjoin!\n");
        return -1;
    }
    

}

void *PseudoTerminal()
{
    pthread_t ThreadId = pthread_self();  

    FILE *PseudoTerm = fopen("/dev/pts/1","w");
    FILE *LogF = fopen("log.txt","r");

    printf("#--Second Child is operating--#\n");

    fprintf(PseudoTerm,"Printing desired Thread process Id of Second Child: [%u]\n");
    fprintf(PseudoTerm,"Printing desired process Id of Second Child's Parent: [%u]\n",ThreadId,(unsigned long)getpid());

    char MyBuff[512];
    struct stat LogFile;

    for (int i = 0; i < 4; i++)
    {

        long PositionV = ftell(LogF);
        stat("log.txt",&LogFile);

        if (LogFile.st_size == PositionV)
        {

            sleep(3);
            continue;

        }

        else
        {

            i = 0;

            fgets(MyBuff,512,LogF);
            fprintf(PseudoTerm,"%s\n",MyBuff);

        }
        
        
    }

    printf("Terminating Process due to inactivity...\n");

    pthread_exit(0);

}

void *ResultPrint()
    {

        pthread_t ThreadId = pthread_self();
        pid_t Child = getpid();

        char MyBuff[512];
        int FileDesc = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC , S_IRUSR | S_IWUSR);

        printf("#--First Child is operating--#");
        printf("Please give your input for the process:\n");


        while (fgets(MyBuff,512,stdin) != NULL)
        {

            int i = 0;

            while (MyBuff[i] != '\n')
            {
                i++;
            }

            write(FileDesc,&MyBuff,i+1);
            
        }
        
        pthread_exit(0);

    }