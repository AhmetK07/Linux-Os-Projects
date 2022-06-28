//1900005528 AhmetKaanMemioglu
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int statement = 0;

    char *EnvId = getenv("USER");
    char *WorkId = getcwd(WorkId,64);

    pid_t ChildId = getpid();
    pid_t ParentId = getppid();

    pid_t ProcessCreatedByParent;
    pid_t ReturnProcess = fork();



    if (ReturnProcess == 0)
    {
        FILE *dir_ = freopen("Result.txt","w+",stdout);

        printf("Your ProcessId: %d\n",getpid());
        printf("Your Parent processId: %d\n",getppid());

        char*argv[] = {"./copy",NULL};

        fprintf(stderr,"Loading up the copy process.");
        printf("\n");

        execv("/home/a1900005528/prj3/copy",argv);

        exit(statement);

    }

    else
    {
        printf("Your current Username: %s\n",EnvId);
        printf("Your current Working directory: %s\n",WorkId);
        printf("#____________________________________#\n");
        printf("Your child process id: %d\n",ChildId);
        printf("Your parent process id: %d\n",ParentId);
        printf("#____________________________________#\n");

         ProcessCreatedByParent = wait(&statement);
    }

    if (ReturnProcess == -1)
    {
        perror("Couldn't find child process.");
        return -1;

    }

   
    printf("The child process that have been created by parent: %d\n",ProcessCreatedByParent);

}