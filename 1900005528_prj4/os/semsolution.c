#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

sem_t fileopen;
sem_t recnb;

int createlive = 0;

void *create();

void *list();

int main()
{
    pthread_t ShowThreads;
    pthread_t CreateThreads;


    sem_init(&recnb,0,1);
    sem_init(&fileopen,0,1);

    if (pthread_create(&ShowThreads,NULL,create,NULL)!=0)
    {
        perror("Failed to initialize threads...");
        return -1;
    }
    if (pthread_create(&CreateThreads,NULL,list,NULL)!=0)
    {
        perror("Failed to initialize threads...");
        return -1;
    }
    if (pthread_join(CreateThreads,NULL)!=0)
    {
        perror("Failed to initialize threads...");
        return -1;
    }


    return 1;

}


void *create()
{
    FILE *FileDesc = fopen("record.txt","w");

    char MyBuff[512];
    createlive = 1;

    sleep(2);

    sem_post(&fileopen);

    do
    {
        fgets(MyBuff,512,stdin);
        sleep(1);
        if (!feof(stdin))
        {
            fprintf(FileDesc,"%s",MyBuff);
            fflush(FileDesc);
            sem_post(&recnb);
        }
        
    } while (!feof(stdin));

    createlive = 0;
    fclose(FileDesc);
    pthread_exit(0);
    
}

void *list()
{

    FILE *TerminalPost;
    FILE *Record;

    char MyBuff[256];

    while ((TerminalPost = fopen("/dev/pts/1","w")) == NULL) //looping until another terminal opens.
    {
        perror("Your 2nd terminal is not opened awaiting its launch...");

        sleep(1); //Warning and sleeping.

    }

    sem_wait(&fileopen);

    while ((Record = fopen("record.txt","r")) == NULL)
    {
        sleep(1);
    }

    do
    {
        while (sem_trywait(&recnb) == 0)
        {
            fgets(MyBuff,256,Record);
            sleep(2);
            fprintf(TerminalPost,"%s",MyBuff);
        }
        fseek(Record,0,SEEK_END);
        sleep(1);

        
    } while (createlive > 0);

    printf("\n Creation of Threads are completed now flushing them..");

    fflush(Record);
    fclose(Record);
    fclose(TerminalPost);


    pthread_exit(0);
    
    
}