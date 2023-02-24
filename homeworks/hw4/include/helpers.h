#ifndef HELPERS_H
#define HELPERS_H
#include <stdlib.h>
#include <stdint.h>
//myincludes
#include "icsmm.h"


/* Helper function declarations go here */
//hw4 helpers
size_t calcminpayloadsize(size_t size);

int calcpagenumbers(int ablocksize,int pagesallocedalready);

void * findinlist(ics_free_header * afreelisthead,size_t theblocksize);

void insertatheadoflist(ics_free_header ** afreelisthead,void * afreeheaderpointer);

void * removefromlist(ics_free_header ** afreelisthead,void * afreeheaderpointer);

#endif
