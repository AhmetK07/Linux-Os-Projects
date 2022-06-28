//1900005528- Ahmet Kaan Memioğlu

//

/* pwd_sort.c
   - sorts ./tstpwd file a local copy of /etc/passwd
	        in ascending order of account name field
   - assumes that "/etc/passwd":
       max.record length is 1023 bytes
       max. number of records is 100
   - uses stream I/O API 
*/
#include <stdio.h>
#include <stdlib.h>
// actcmp returns 0 if s==t, <0 if s<t, >0 if s>t
int actcmp(char *s, char *t)
{
	for( ; *s == *t ; s++, t++)
		if ( *s == ':') return 0; // s = t
	if ( *s == ':') return -1 ;   // s < t
	if ( *t == ':') return +1 ;   // s > t
	return *s - *t ;
}
         
#define MAXRLEN  1023  // max.record lengt in "/etc/passwd"
#define MAXRNUM  100   // max.# of records in "/etc/passwd"     

int main(int argc, char *argv[])
{	
	//char SortTab [MAXRNUM+1] [MAXRLEN+1]; // user account records
	//int  SortIdx [MAXRNUM+1]; // sorted indices of user accounts
	int nbusr, i, j, tmp;
	FILE *fppwd;		//  -->   ./passwd
	FILE *test;  //For our tstpwd.srt file we have made a file pointer.

	if ( ( fppwd= fopen("./tstpwd", "r") ) == NULL)
		{ perror("./tstpwd: "); return 1; } 

	// fgets reads a ./passwd record including '\n'
    //       stores it as a string adding by a '\0'

	int LineCounter;

	LineCounter = atoi(argv[1]);

    int *SortIdx = (void *)malloc(LineCounter* sizeof(int)); //Here in these lines we initialize memory for our variables.
    
    char (*SortTab)[MAXRNUM] = (void *)malloc(LineCounter*MAXRLEN+1 *sizeof(char));

	//char **SortTab = (char **)malloc(MAXRNUM * MAXRNUM * sizeof(char *));

    
	i = 0;    
	for ( nbusr= 0; nbusr < LineCounter ; nbusr++ ) {
		SortIdx[nbusr] = nbusr;  // set index to records order
		if (fgets(*(SortTab+nbusr), MAXRLEN, fppwd) == NULL )
			break;
	}

	// bubble sort TabIdx in ascending order of account names
	for (i = 0; i < nbusr -2 ; i++)
		for (j = i+1; j < nbusr-1; j++)
			if (actcmp ( (*(SortTab+SortIdx[i])),  (*(SortTab+SortIdx[j]))) > 0){
				tmp = SortIdx[i];
				SortIdx[i] = SortIdx[j];     // swap entries i and j
				SortIdx[j] = tmp;
			} 
	
	// display account records in ascending order
	test = fopen("tstpwd.srt","w"); //This line opens a file named tstpwd.srt 
	for (i=0; i < nbusr; i++)
	{
		printf("acct[%d]: %s", i, (SortTab+SortIdx[i])) ;
		fprintf(test ,"acct[%d]: %s", i, *(SortTab+SortIdx[i])); // And here in this line we make so that our fprintf function prints the contents to the new opened file .
    }
    
    free(SortIdx);
    
    free(SortTab);
    
	return 0;
}
