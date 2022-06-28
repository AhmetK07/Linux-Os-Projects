//1900005528 Ahmet Kaan Memioğlu

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>

#define BufferSize 4096


int Compare(char one,char two) //For comparing the characters in the bufferarray.

{
   if (one == two)
   {
      return 1;
   }
   else
   {
      return 0;
   }
   
}

int PrintO(char* MyBuffer,int index) //For printing the Home directory and login shell respectively.

{
   
   OUT_LOOP:

   if (MyBuffer[index] != '/')
   {
      index++;

      goto OUT_LOOP;
   }
   
   printf("\n Displaying your desired home directory:");

   bool flag =0;

   while (MyBuffer[index] != '\n')
   {
      if(flag)
      {
         printf("\n Displaying your desired login Shell: ");
         flag = false;
      }

      putc(MyBuffer[index],stdout);

   if (MyBuffer[index+2]== ':')
   {
      flag = 1;
      putc(MyBuffer[index+1],stdout);
      index +=3;
      continue;
   }

   index++;
   
   }

   printf("\n");
   return index;
}



 int main(int argc, char* argv[])
 {
     
    int users;

    int UserName;

    int column= 1;

    //int test; //For testing purposes i have created another file desctiptor inorder to see whats going on.

    char MyBuffer[BufferSize];

    char UserBuffer[BufferSize];

    if (argv[1]==NULL)

    {
       fputs("You haven't entered any arguements to the program to work.",stderr); //If the user does not enter any arguements to look for it gives an error.

       exit(0);
    }

    users = open("password.srt", O_RDONLY); //We open the file in read only to read its values into our buffer array.

    //test = open("deneme.txt", O_CREAT | O_WRONLY); // This line used to be my backup txt file to look up its contents.

    UserName = getlogin_r((char*)UserBuffer,BufferSize); //this line sets the current user name to the UserName variable.
   
    printf("Useraccount's name: %s  | | | ", UserBuffer);

    printf("Query program name: %s \n", argv[0]);

    printf("\n");

    read(users,&MyBuffer,BufferSize); //Here we read the content of the Mybuffer with the help of the file descriptor.

    //write(1,&MyBuffer,BufferSize); //I used to used this line to display our MyBuffer's contents.

    char** temp = argv;

    for (int i = 0,j = 0,word = 0;i < 4096  || MyBuffer[i] != '\0'; i++)
    {
       if (MyBuffer[i]==':') //We look for the specific charachters such as : and then make our comparings after.
       {
          if (Compare(MyBuffer[i+2],*(*(temp+column)+j)))
          {
             word++; //If the word same after comparing we increment the word value.

             while (MyBuffer[i+2]!='\n'||MyBuffer[i+2]!=':')
             {
                i++; //We increment the indexes.

                j++;

                Check_Again:
                if (Compare(MyBuffer[i+2],*(*(temp+column)+j)))
                {
                   word++; //we check again and increment the word for another comparison later.
                }
                
                else
                {
                   VISIT:

                   if (word == strlen(argv[column])) //This works when we have found the exact word the users input.
                   {
                      i = PrintO(MyBuffer,i+2); //Then we go ahead and print the home directory and login shell of the user.

                      goto NEXTP;
                   }

                   if (argv[column+1]!=NULL) //In these lines if the user have typed more than one input we are looking for them again too.
                   {
                      column++;

                      j=0;

                      if (word >0)
                      {
                         i -= word;

                         word = 0;
                      }

                      goto Check_Again;
                      
                   }
                   
                   goto NEXTP;

                }
             }
             
          } 
          
          else
          
          {
             goto VISIT;

             NEXTP:

             column=1;

             j=0;

             word=0;

             while (true) //This loop allows us to look for the character [ so it finds the begging of the line and then registers if its been getting close to the end of file.
             {
                if (MyBuffer[i]=='[')
                {
                   break;
                }

                if (MyBuffer[i]=='\0')
                {
                   fputs("\n You have reached the end of the file. \n",stderr); //And here we give the user its reached the end of the file.

                   exit(1);
                }
                
                i++;

             }
             
          }


          
       }
       
    }
   

 }


   



 
      

 

 
