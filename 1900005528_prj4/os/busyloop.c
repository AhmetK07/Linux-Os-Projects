#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>

int LoopCounter = 0;

void *CreateThread(void *Tname);

void *DisplayThread(void *Tname);

int main()
{

    pid_t Process = getpid();
    pid_t ParentProcess = getppid();

    pthread_t Main = pthread_self();

    pthread_t Thread1;
    pthread_t Thread2;

    printf("\n#____________________________________________________________________________#\n");

    printf("Main Program --> Your desired Process Id = %u\n", Process);
    printf("Main Program --> Your desired id of the Parent's Parent = %u\n", ParentProcess);
    printf("Main Program --> Your desired Thread Id = %u\n", Main);

    printf("\n#____________________________________________________________________________#\n");

    pthread_create(&Thread1,NULL,CreateThread,"TextDoc.txt");
    pthread_create(&Thread2,NULL,DisplayThread,"TextDoc.txt");

    pthread_join(Thread1,NULL);
    pthread_join(Thread2,NULL);



}

void *CreateThread(void *Tname)
{
    int FileDescriptor = open((char *)Tname, O_CREAT | O_WRONLY | O_TRUNC | O_SYNC | S_IWUSR | S_IRUSR);

    pid_t Process = getpid();
    pid_t ParentProcess = getppid();
    
    pthread_t ThreadId = pthread_self();

    char MyBuff[256];

    printf("\n#______________________________________________________________________#\n");
    printf("Thread Creation --> Your desired Process Id = %u\n");
    printf("Thread Creation --> Your desired Process of the Parent's Parent = %u\n",ParentProcess);
    printf("Thread Creation --> Thread Id = %u\n",ThreadId);
    printf("\n#______________________________________________________________________#\n");

    while (fgets(MyBuff,256,stdin) != NULL)

    {
        int SizeOf = strlen(MyBuff);

        write(FileDescriptor,MyBuff,SizeOf);

        LoopCounter++;

    }

    pthread_exit(0);

}

void *DisplayThread(void *Tname)
{

    char MyBuff2[512];
    int TempCount = LoopCounter;

    FILE *PseudoTerm = fopen("/dev/pts/1", "w");
    FILE *TextDoc = fopen((char *)Tname, "r");


    pid_t Process = getpid();
    pid_t ParentProcess = getppid();

    for (int i = 0; i < 4; i++)
    {
        if (TempCount == LoopCounter)
        {
            sleep(3);
            i++;
            continue;

        }

        else 
        {
            while(fgets(MyBuff2,512,TextDoc) != NULL)
            {
                fprintf(PseudoTerm,"%s\n",MyBuff2);
            }

            i = 0;

            TempCount = LoopCounter;

            fseek(TextDoc,0,SEEK_END);

            continue;

        
        }
        
    }

    printf("DisplayThread --> Closing Your program\n");

    fclose(PseudoTerm);
    fclose(TextDoc);

    pthread_exit(0);
    

}