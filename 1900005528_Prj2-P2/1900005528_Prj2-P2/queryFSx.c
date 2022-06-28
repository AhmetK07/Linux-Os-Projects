//1900005528 Ahmet Kaan Memioğlu

/* queryFSx.c, is an Indexed File Sysytem query program that: 
   - allocates Index Table dynamically using the size of index file "./pwd.idx"
   - computes the size of the record length of "./pwd.idx" dile
   - reads account names to look for from stdin until EOF
   - searches the account in the Index Table
     if found uses sets "./fwd.fix" file position & retrieve the record
	 and displays user's home directory

   - uses Low-level I/O API access "./pwd.idx" and "./pwd.fix" files 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>    // Low Level I/O API read. write, seek
#include <fcntl.h>     // Low Level open
#include <sys/stat.h>
#include <grp.h>



typedef struct { // idx file record structure
	char actname [32+1];   // account name string
	long int pwdoffset;    // passwd file offset
} IdxRec, *pIdxRec;


void IDXFUNC(long int offset,off_t st_size);  

int main(int argc, char *argv[])
{
	long int pos;	//  byte offset in "./pwd.fix"

	int pwdrel; 		//  length of record in "./pwd.fix" file
	int fdpwd;			//  file descriptor of "./pwd.fix"
	int fdidx; //  file descriptor of "./pwd.idx" 

	pIdxRec IdxTab;	//  index table pointer 
	char uname[32+1+1];
	char *buf;	

	struct stat idx, fix; // Data structures used by stat function
	struct group *pgr;   // pointer to data structure returnes by getgrid 

	int nbusr, i, j;
	
	// get the size of the index file and compute the number of entries
	if (stat("./pwd.idx", &idx) < 0)
		{ puts("stat error"); return 1;}
	nbusr = idx.st_size / sizeof(IdxRec); // nb. of accounts
	// allocate the idx structure table IdxTab [nbsusr]
	if ( ( IdxTab = malloc(idx.st_size) ) == NULL) 
		{ puts("buffer allocation error"); return 2; }
	
	// get the size of data file and compute the record length
	if (stat("./pwd.fix", &fix) < 0)
		{ puts("stat error"); return 3;}
	pwdrel = fix.st_size / nbusr;    // "./pwd.fix" record length  
	// allocate input buffer to read "./pwd.fix" records
	if ( ( buf = malloc(pwdrel) ) == NULL) 
		{ puts("buffer allocation error"); return 4; }
	
	printf("\npwd.fix file has <%d> records of <%d> bytes\n", nbusr,pwdrel);
/* -------------------------------------------------------------------------*/   	

fdidx = open("./pwd.idx",O_RDONLY);

read(fdidx,(void*)IdxTab,idx.st_size);

for (int Count = 0; Count < 36; Count++)
{
	for (int Counter = 0; argv[Counter] != NULL; Counter++)
	{
		int length = strlen(argv[Counter]);
		if (memcmp(IdxTab[Count].actname,argv[Counter],length)==0)
		{
			IDXFUNC(IdxTab[Count].pwdoffset,fix.st_size);
		}
		
	}
	
}



	return 0;
}

void IDXFUNC(long int offset,off_t st_size)
{
	char *pwd = (void*)malloc(st_size);
	int fdpwd = open ("./pwd.fix",O_RDONLY);

	read(fdpwd,pwd,st_size);
	printf("\n");
	int i = 0;
	while (pwd[offset+i] != '/')
	{
		i++;
	}
	printf("\n");
	printf("#_________________________________________#");
	printf("\n");
	printf ("\n User's Desired Home Directory: ");

	while (pwd[offset+i] != ':')
	{
		putc(pwd[offset+i],stdout);
		i++;
	}

	printf("\n");
	printf(" \n User's Desired Login Shell: ");

	i++;
	while (pwd[offset+i] != '\n')
	{
		putc(pwd[offset+i],stdout);
		i++;
	}
	
	printf("\n\n");
	printf("#_________________________________________#");
	printf("\n");
	
}
