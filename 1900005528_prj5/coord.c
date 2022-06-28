//Ahmet Kaan Memioğlu 1900005528

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <error.h>
#include <semaphore.h>
#include <pthread.h>

#define nbslots 4

typedef struct 
{
    char type;
    int amount;
    int newball;
    int oldbal;
} LogRec, *pLogRec;

LogRec LogCirq[nbslots];

sem_t s_circq;
sem_t s_full;
sem_t s_free;

pthread_mutex_t BalanceMuteX;

char AtmMode;

int in = 0;
int out = 0;

int Balance = 0;

void *Arch();
void *Bill(void *PseudoTerm);
void *Pay(void *PseudoTerm);
void *Atm();


int main()
{
    FILE* PseudoTerm;

    sem_init(&s_circq,0,1);
    sem_init(&s_full,0,0);
    sem_init(&s_free,0,nbslots);

    pthread_t atmThread;
    pthread_t payThread;
    pthread_t billThread;
    pthread_t archThread;

    pthread_mutex_init(&BalanceMuteX,NULL);

    while ((PseudoTerm = fopen("/dev/pts/1","w")) == 0)
    {
        printf("\n Please create a new terminal window \n");
    }

    if (pthread_create(&atmThread,NULL,Atm,NULL) == -1)
    {
        perror("Failed to initialize thread..\n");
        return -1;
    }

    printf("\n Your thread id for [ATM] --> [%u]\n",atmThread);

    if (pthread_create(&payThread,NULL,Pay,PseudoTerm) == -1)
    {
        perror("Failed to initialize thread..\n");
        return -1;
    }

    printf("\n Your thread id for [PAY] --> [%u]\n",payThread);

    if (pthread_create(&billThread,NULL,Bill,PseudoTerm) == -1)
    {
        perror("Failed to initialize thread..\n");
        return -1;
    }

    printf("\n Your thread id for [BILL] --> [%u]\n",billThread);

    if (pthread_create(&archThread,NULL,Arch,NULL) == -1)
    {
        perror("Failed to initialize thread..\n");
        return -1;
    }

    printf("\n Your thread id for [ARCH] --> [%u]\n",archThread);


    if (pthread_join(atmThread,NULL) == -1)
    {
        perror("\n atmThread has failed to initialize\n");
        return -1;
    }

    if (pthread_join(billThread,NULL) == -1)
    {
        perror("\n billThread has failed to initialize\n");
        return -1;
    }

    if (pthread_join(payThread,NULL) == -1)
    {
        perror("\n payThread has failed to initialize\n");
        return -1;
    }

    if (pthread_join(archThread,NULL) ==-1)
    {
        perror("\n archThread has failed to initialize\n");
        return -1;
    }

    fflush(NULL);
    fclose(PseudoTerm);

    return(0);
    
    

}

void *Atm()
{
    
    AtmMode = 1;

    sleep(1);
    
    printf("\n Atm is ready to go, firing up the code...");

    int input;

    while (1)
    {
        printf(" \n Please enter a value to insert money for atm: ");

         int eof = scanf("%d", &input);

          if (eof == EOF)
        {
            
            printf("You have reached the eof terminating the program ...");

            AtmMode = 0;

            pthread_exit(0);

            break;

        }

        else if (input < 0 && (Balance - input) < 0)
        {
            printf("\n You have typed a negative amount this is not possible \n");

            continue;

        }

        else
        {
            pthread_mutex_lock(&BalanceMuteX);
            LogCirq[in].type = 'A';
            LogCirq[in].oldbal = Balance;

            Balance += input;

            LogCirq[in].newball = Balance;
            in = (in+1) % nbslots;
            pthread_mutex_unlock(&BalanceMuteX);

            printf("Your bank account's balance: [%d]",Balance);
        }

    }
    
}


void *Pay(void *PseudoTerm)
{
    fprintf((FILE*)PseudoTerm,"\n Pay thread has fired up, creating its contents. \n");

    while (1)
    {
        if (!AtmMode)//this line
        {
            fprintf((FILE*)PseudoTerm, "Closing the pay thread.. \n");
            pthread_exit(0);
        }

        sleep(12);

        pthread_mutex_lock(&BalanceMuteX);
        LogCirq[in].type = 'P';
        LogCirq[in].oldbal = Balance;

        Balance += 10;

        LogCirq[in].newball = Balance;
        LogCirq[in].amount = 10;
        in = (in+1) % nbslots;
        pthread_mutex_unlock(&BalanceMuteX);

        printf("Your bank account's balance: [%d]",Balance);
        
        fprintf((FILE*)PseudoTerm, "\n Your bank account's balance: [%d]",Balance);

        
    }
}


void *Bill(void *PseudoTerm)
{
    fprintf((FILE*)PseudoTerm, "\n Bill thread has been created..");

    while (1)
    {
        if (!AtmMode)
        {
            fprintf((FILE*)PseudoTerm, "Closing the Bill thread ...\n");
            pthread_exit(0);
        }

        sleep(3);
        sem_wait(&s_free);

        pthread_mutex_lock(&BalanceMuteX);
        LogCirq[in].type = 'B';
        LogCirq[in].oldbal = Balance;

        Balance += -1;

        LogCirq[in].newball = Balance;
        LogCirq[in].amount = -1;
        in = (in+1) % nbslots;
        pthread_mutex_unlock(&BalanceMuteX);

        fprintf((FILE*)PseudoTerm, "\n Bill: -1 | Your Current Balance: [%d]\n",Balance);

        sem_post(&s_full);

        
    }

    perror("\n An error has occured during pay thread...");

    fprintf((FILE*)PseudoTerm, "\n You have reached the eof terminating the program ...");

    pthread_exit(0);
}

void *Arch()
{
    FILE *PseudoTerm;

    while ((PseudoTerm = fopen("/dev/pts/2","w")) == 0)
    {
        sleep(2);

        printf("\n Please open your third terminal inorder to continue..\n");
    }

    while (1)
    {
        if (!AtmMode)
        {
            fprintf(PseudoTerm,"\n Closing Arch function.. \n");

            break;
        }

        sem_wait(&s_full);

        pthread_mutex_lock(&BalanceMuteX);

        fprintf(PseudoTerm, "\n%c\t%d\t%d\t%d\n",LogCirq[out].type,LogCirq[out].amount,LogCirq[out].oldbal,LogCirq[out].newball);

        out = (out + 1) % nbslots;

        pthread_mutex_unlock(&BalanceMuteX);

        sem_post(&s_free);
        
    }

    fflush(PseudoTerm);
    fclose(PseudoTerm);
    
    pthread_exit(0);
    
    
}

