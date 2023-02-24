#include "helpers.h"
#include "debug.h"
//myinclide
#include "icsmm.h"

/* Helper function definitions go here */
size_t calcminpayloadsize(size_t size)
{
    //
    int intdiv = (size/16); // the ceil funct

    if(size%16 != 0)
    {
        intdiv++;
    }

    printf("the intdiv %d\n",intdiv);

    

    return 16*intdiv;
}

int calcpagenumbers(int ablocksize,int pagesallocedalready)
{
    if(pagesallocedalready == 0)
    {

        //need 4064 for first page 4096 for next subsquent
        if(ablocksize <= 4080)
        {
            return 1;
        }
        else
        {
            int thepagescalc = 1;
            //subtract 
            int payloadwithoutfpage  = ablocksize-4080;

            if(payloadwithoutfpage % 4096 == 0)
            {
                thepagescalc += payloadwithoutfpage/4096;
            }
            else
            {
                thepagescalc += payloadwithoutfpage/4096 + 1;
            }

            printf("int page calc div %d\n",thepagescalc);

            return thepagescalc;



        }
        

    }
    else
    {
        //for the case where a page is already allocated
        //offset by 8 byte for the footer and epilogue at end and header is not included
        ablocksize -= 8;

        int pagecount = 0;
        

        while(ablocksize > 0)
        {
            pagecount++;
            ablocksize -= 4096;
        }

        return pagecount;

    }


    
}

void * findinlist(ics_free_header * afreelisthead,size_t theblocksize)
{
    //check if initial null
    if(afreelisthead == NULL)
    {
        return NULL;
    }

    //go through the list to find it
    //test return NULL
    // return NULL;

    ics_free_header * themvfreeheadprt = afreelisthead;

    while(themvfreeheadprt != NULL)
    {
        printf("moving the pointer\n");

        ics_header_print(themvfreeheadprt);

        size_t thecurrentblksize = (((themvfreeheadprt)->header)).block_size;

        printf("the block size obtained is %ld\n",thecurrentblksize);

        //check if the current node satisfy the blk size given!
        if(thecurrentblksize >= theblocksize)
        {
            //return the current node
            return themvfreeheadprt;
        }

        //move pointer to next node
        themvfreeheadprt = themvfreeheadprt->next;



    }

    
    

    //satisft error
    return NULL;
}


void insertatheadoflist(ics_free_header ** afreelisthead,void * afreeheaderpointer)
{
    //check if null
    if(*afreelisthead == NULL)
    {
        printf("----------------------Inserting at front\n");

        ics_free_header * freehedptr = afreeheaderpointer;

        //set the prev and next to null
        freehedptr->next = NULL;
        freehedptr->prev = NULL;

        //insert at front
        *afreelisthead = afreeheaderpointer;

        
    }
    else
    {
        //the list has something in it
        ics_free_header * storeoldlistnodes = *afreelisthead;

        ics_free_header * newnodetoinsert = afreeheaderpointer;

        //set oldlistnode prev to the insert node
        storeoldlistnodes->prev = newnodetoinsert;

        //set newnodes next to old list node
        newnodetoinsert->next = storeoldlistnodes;
        //set the prev to null
        newnodetoinsert->prev = NULL;

        //update the freelist ptr
        *afreelisthead = afreeheaderpointer;



    }
}

void * removefromlist(ics_free_header ** afreelisthead,void * afreeheaderpointer)
{
    //remove the node that is pointed to by the afreeheaderpointer
    //check for case where a freelisthead is NULL and afreeheader pointer is null
    if(*afreelisthead == NULL || afreeheaderpointer == NULL)
    {
        return NULL;
    }

    ics_free_header * givenfreeheadptr = afreeheaderpointer;
    //case where there both prev and next is null meaning 1 node

    if((givenfreeheadptr->next == NULL) && (givenfreeheadptr->prev == NULL))
    {
        printf("only one node in list to remove\n");

        //set the listhead to null
        *afreelisthead = NULL;
        //return the node
        return afreeheaderpointer;
    }
    else if(givenfreeheadptr->prev == NULL)
    {
        //prev is null node is at the front of list

        printf("removing from the front\n");

        //store the nextnode
        ics_free_header * nextfreenode = givenfreeheadptr->next;

        //set next node prev to null
        nextfreenode->prev = NULL;

        //set this current node next to null
        givenfreeheadptr->next = NULL;

        //set the freelist head to next node
        *afreelisthead = nextfreenode;

        //return the current node
        return afreeheaderpointer;

    }
    else if(givenfreeheadptr->next == NULL)
    {
        //next is null so node is at back
        printf("removing from the back\n");

        //store the prevnode
        ics_free_header * prevfreenode = givenfreeheadptr->prev;

        //set prev node next to null
        prevfreenode->next = NULL;

        //set current prev to null
        givenfreeheadptr->prev = NULL;

        //return the current node
        return afreeheaderpointer;

    }
    else
    {
        //the option that the node is in between two nodes
        printf("removeing a node that is inbetween two nodes\n");

        //store the next node
        ics_free_header * nextfreenode = givenfreeheadptr->next;
        //store the previous node
        ics_free_header * prevfreenode = givenfreeheadptr->prev;

        //set prev's next to nextnode
        prevfreenode->next = nextfreenode;
        //set next's prev to prevnode
        nextfreenode->prev = prevfreenode;

        //set current node next and prev to null
        givenfreeheadptr->next = NULL;
        givenfreeheadptr->prev = NULL;

        //return the current node
        return givenfreeheadptr;


    }
   




    //temporary
    return NULL;
}
