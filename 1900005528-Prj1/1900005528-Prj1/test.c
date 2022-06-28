//1900005528 Ahmet Kaan Memioğlu
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* slink.c program 
    - reads the std.txt file stored in the same directory
    - creates a singly linked list sorted in ascending order */

// type definitions
#define IDL 10  //id field length
#define NML  7  //name field length
typedef struct List_elm { // Linked List item
    char id[IDL];
    char name[NML];
    struct List_elm *next;
    } List_elmDef, *pList_elmDef;

int main()
{ 
    FILE *fp;
    pList_elmDef new, p;   // pointers to linked list elements
   // defining empty single linked list head
    List_elmDef Lhead = {"123456789","abcdef", NULL};
    printf("\nLhead inits: %s < %s>  %p\n", Lhead.id, Lhead.name,Lhead.next);

    // open std.txt file
    if (( fp = fopen("./std.txt","r")) == NULL)
        { perror("std.txt open error"); return 1;}

    // allocate space for a new element
    if ( (new = (pList_elmDef) malloc(sizeof(List_elmDef))) == NULL)
          { perror("malloc error"); return 2;}
    // --------------- read "std.txt until EOF -------------------
    // fgets reads up to size-1 or a new line character (LF)
    //       terminates input characters with a LF +  null '\0" char
    //       returns NULL pointer on end-of-file or error 
    while ( fgets( (char *) new, sizeof(List_elmDef), fp) != NULL ) {
        printf("record read: %s", new);
        // link new record in ascending order
        p = &Lhead; 
        while ( (p->next !=NULL) &&
                (memcmp( &(new->name), &((p->next)->name), NML) > 0) )
              p = p->next;  // continue to search

        new->next = p->next ;
        p->next = new;   

        // allocate space for the new element
        if ( (new = (pList_elmDef) malloc(sizeof(List_elmDef))) == NULL)
            { perror("malloc error"); return 3;}     
    } 
    
    // print sorted list
    int i; printf("\n---Records sorted in ascending order of ID field\n");
    for( p = Lhead.next; p != NULL;  p= p->next) {
        for (i=0; i < IDL; i++) printf("%c", p->id[i]); printf("\t");
        for (i=0; i < NML; i++) printf("%c", p->name[i]); printf("\n"); 
        }   
    fclose(fp);
    return 0;
}
 
