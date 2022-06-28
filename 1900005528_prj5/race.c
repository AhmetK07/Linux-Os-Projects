//Ahmet Kaan Memioğlu 1900005528

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <semaphore.h>
#include <pthread.h>

int Balance = 0;

sem_t RunAtm;
sem_t RunBill;
sem_t RunPay;

char AtmMode = 1;

pthread_mutex_t BalanceMuteX;

void *Bill(void *PseudoTerm);
void *Pay(void *PseudoTerm);
void *Atm();

int main()
{

    FILE* PseudoTerm;

    sem_init(&RunAtm,0,1);
    sem_init(&RunBill,0,1);
    sem_init(&RunPay,0,1);

    pthread_t atmThread;
    pthread_t billThread;
    pthread_t payThread;

    while ((PseudoTerm = fopen("/dev/pts/1", "w")) == 0)
    {
        printf("\n Please create a new terminal window \n");

    }

    if (pthread_create(&atmThread,NULL,Atm,NULL) == -1)
    {
        perror("Failed to initialize thread..\n");
        return-1;
    }

    printf("\n Your thread id for [ATM] --> [%u]\n",atmThread);

    if (pthread_create(&billThread,NULL,Bill,PseudoTerm) == -1)
    {
        perror("Failed to initialize thread..\n");
        return-1;
    }

    printf("\n Your thread id for [BILL] --> [%u]\n",billThread);

    if (pthread_create(&payThread,NULL,Pay,PseudoTerm) == -1)
    {
        perror("Failed to initialize thread..\n");
        return-1;
    }

    printf("\n Your thread id for [PAY] --> [%u]\n",payThread);

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

    fflush(NULL);
    fclose(PseudoTerm);

    return 0;

}

void *Atm()
{

    AtmMode = 1;

    sleep(2);

    printf("\n Atm is ready to go, firing up the code...");

    int input;

    sem_post(&RunAtm);
    sem_post(&RunBill);
    sem_post(&RunPay);

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

            Balance += input;

            pthread_mutex_unlock(&BalanceMuteX);

            printf("Your current balance is : [%d] ",Balance);

        }

    }
    

}

void *Pay(void *PseudoTerm)
{

    fprintf((FILE*)PseudoTerm, "\n Pay thread has fired up, creating its contents.\n");
    sem_wait(&RunPay);

    while (1)
    {
        if (!AtmMode)//this line
        {
            fprintf((FILE*)PseudoTerm, "Closing the pay thread.. \n");
            pthread_exit(0);
        }

        sleep(12);

        pthread_mutex_lock(&BalanceMuteX);
        Balance += 10;
        pthread_mutex_unlock(&BalanceMuteX);
        
        fprintf((FILE*)PseudoTerm, "\n Your bank account's balance: [%d]",Balance);

        
    }
    

}


void *Bill(void *PseudoTerm)
{
    fprintf((FILE*)PseudoTerm, "\n Bill thread has been created..");
    sem_wait(&RunBill);

    while (1)
    {
        if (!AtmMode)
        {
            fprintf((FILE*)PseudoTerm, "Closing the Bill thread ...\n");
            pthread_exit(0);
        }

        sleep(3);

        pthread_mutex_lock(&BalanceMuteX);
        Balance += -1;
        pthread_mutex_unlock(&BalanceMuteX);

        fprintf((FILE*)PseudoTerm, "\n Bill: -1 | Your Current Balance: [%d]\n",Balance);
        
    }

    perror("\n An error has occured during pay thread...");
    fprintf((FILE*)PseudoTerm, "\n You have reached the eof terminating the program ...");

    pthread_exit(0);
    
}






