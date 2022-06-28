//1900005528 AhmetKaanMemioglu
#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main()
{

    int statement = 0;

    char *EnvId = getenv("USER");
    char *WorkId = getcwd(WorkId,64);

    pid_t ChildId = getpid();
    pid_t ParentId = getppid();

    FILE *dir_ = freopen("Test.txt","w",stdout); //GNU C library page 267.

    printf("Your current Username: %s\n",EnvId);
    printf("Your current Working directory: %s\n",WorkId);
    printf("#____________________________________#\n");
    printf("Your child process id: %d\n",ChildId);
    printf("Your parent process id: %d\n",ParentId);
    printf("#____________________________________#\n");

    pid_t ParentsChildID;
    pid_t ReturnProcess = fork();

    if (ReturnProcess == 0)
    {
        char*argv[] = {"./copy",NULL};

        fprintf(stderr,"Loading up the copy process.");
        printf("\n");

        execv("/home/a1900005528/prj3/copy",argv);
        exit(statement);

    }

    if (ReturnProcess == -1)
    {
        perror("Couldn't find child process.");
        return -1;

    }

    ParentsChildID = wait(&statement);

    fclose(dir_);

    return 0;

    
}