#include "icsmm.h"
#include "debug.h"
#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * The allocator MUST store the head of its free list in this variable. 
 * Doing so will make it accessible via the extern keyword.
 * This will allow ics_freelist_print to access the value from a different file.
 */
ics_free_header *freelist_head = NULL;

int allocpagecount = 0;

//store the begining of the heap
void * beginingofheap = NULL;

void * storenodefortest = NULL;

/*
 * This is your implementation of malloc. It acquires uninitialized memory from  
 * ics_inc_brk() that is 16-byte aligned, as needed.
 *
 * @param size The number of bytes requested to be allocated.
 *
 * @return If successful, the pointer to a valid region of memory of at least the
 * requested size is returned. Otherwise, NULL is returned and errno is set to 
 * ENOMEM - representing failure to allocate space for the request.
 * 
 * If size is 0, then NULL is returned and errno is set to EINVAL - representing
 * an invalid request.
 */
void *ics_malloc(size_t size) { 
    // return NULL; NOTE remove print statements

    //test getting more than 6 pages
    void * thecurrbrk = ics_get_brk();

    if(allocpagecount == 0)
    {
        //set the begining of the heap
        beginingofheap = thecurrbrk;
    }

    // printf("the current breakpoint %p\n",thecurrbrk);

    size_t thecalcminpayloadsz = calcminpayloadsize(size);

    

    //call find in list to find a fit for it
    size_t thereqcalcblksize = thecalcminpayloadsz + 16;
    void * findinlistptr = findinlist(freelist_head,thereqcalcblksize);

    //check if found is null
    if(findinlistptr == NULL)
    {
        //need to get pages for memory

        // //test if last page exists
        // char * theprevpageprt = thecurrbrk;
        // theprevpageprt -= 8;
        //check for header
        // uint64_t theprevepilogue = ((ics_header *) theprevpageprt)->hid;
        // printf("the previous epilogue %lx\n",theprevepilogue);

        int numpagescalc = calcpagenumbers(thereqcalcblksize,allocpagecount);

        printf("getting pages from mem %d\n",numpagescalc);

        void * thebegofheapbfinc = ics_inc_brk(numpagescalc);

        if(thebegofheapbfinc == ((void *)-1))
        {
            //when the request for more pages fails due to max limit
            // printf("warning the heap inc error\n");

            //set errno to ENOMEM
            errno = ENOMEM;

            //return null
            return NULL;


        }

       //need to set up the page
       char * themvheapptr = thebegofheapbfinc;

        if(allocpagecount == 0)
        {
            
            //set up the prologue
            ((ics_footer *) themvheapptr)-> block_size = 1;
            ((ics_footer *) themvheapptr)-> fid = PROLOGUE_MAGIC;
        }

        //set up the headers and footers for empty
        //calc size
        int thecalcsizefreeblk = 0;
        
        if(allocpagecount == 0)
        {
            //first time move it past the prologue
            themvheapptr += 8;
        }
        else
        {
            //move the ptr to the epilogue to set to header
            themvheapptr -= 8;

            //check the prev footer if it is free
            char * theprevfooterptr = themvheapptr - 8;
            printf("the prevfooter size is %d\n",((ics_footer *) theprevfooterptr)->block_size);
            if(isblockfreed(((ics_footer *) theprevfooterptr)->block_size) == 1)
            {
                printf("the prev block before getting page is free \n");

                //get the size from the footer
                size_t theprevfootersize = ((ics_footer *) theprevfooterptr)->block_size;

                //add the size to the calcsizefreeblk
                thecalcsizefreeblk += theprevfootersize;
                printf("the new thecalcsizefreeblk size %d\n",thecalcsizefreeblk);

                //move the prevfooterptr back loweraddress by size-8 due to it pointing at the prevfooter
                theprevfooterptr -= (theprevfootersize - 8);

                //remove the prevfreeheader from freelist
                removefromlist(&freelist_head,theprevfooterptr);

                //set the mvheadptr to the prevfooterpointer which now points at blocks head
                themvheapptr = theprevfooterptr;

                

            }
        }

        //store the freeblock header
        findinlistptr = themvheapptr;

        //set up the free block header
        
        if(allocpagecount == 0)
        {
            thecalcsizefreeblk = 4096 * numpagescalc;

            thecalcsizefreeblk -= 16; //for the prologue and epilogue

        }
        else
        {
            thecalcsizefreeblk += 4096 * numpagescalc;//for the footer a the end
            
        }

        // printf("the calc blk size %ld\n",thereqcalcblksize);
        printf("the calcsizefreeblk after addition %d\n",thecalcsizefreeblk);

        //set up the header
        //alloc header
        ((ics_free_header *) themvheapptr)->next = NULL;
        ((ics_free_header *) themvheapptr)->prev = NULL;

        ((((ics_free_header *) themvheapptr)->header)).block_size = thecalcsizefreeblk|0;
        ((((ics_free_header *) themvheapptr)->header)).hid = HEADER_MAGIC;
        ((((ics_free_header *) themvheapptr)->header)).padding_amount = 0;



        

        //setup the epilogue
        char * theepiloguebrkptr = ics_get_brk();
        // printf("the brak is %p\n",ics_get_brk());
        theepiloguebrkptr -= 8;
        ((ics_header *) theepiloguebrkptr)->block_size = 1;
        ((ics_header *) theepiloguebrkptr)->hid = EPILOGUE_MAGIC;
        ((ics_header *) theepiloguebrkptr)->padding_amount = 0;

        //set up the footer
        theepiloguebrkptr -= 8;
        ((ics_footer *) theepiloguebrkptr)->block_size = thecalcsizefreeblk|0;
        ((ics_footer *) theepiloguebrkptr)->fid = FOOTER_MAGIC;

        // printf("the footerlocation %p\n",theepiloguebrkptr);




        // char * themvheapptrhdft = thebegofheapbfinc;
        // //for the allocated blk to be returned
        // //setup the header
        // if(allocpagecount == 0)
        // {
        //     printf("the alloc page count 0\n");
        //     themvheapptrhdft+=8;
        // }
        // {
        //     //when we already have a page
        //     themvheapptrhdft-=8;

        // }

        // //store in findin list ptr
        // findinlistptr = themvheapptrhdft + 8;
        

        // // void * storetheheader = themvheapptrhdft;
        // // void * storepayloadptr = themvheapptrhdft+8;
        // // ((ics_free_header *) themvheapptrhdft)->next = NULL;
        // // ((ics_free_header *) themvheapptrhdft)->prev = NULL;

        // // ((((ics_free_header *) themvheapptrhdft)->header)).block_size = thereqcalcblksize;
        // // ((((ics_free_header *) themvheapptrhdft)->header)).hid = HEADER_MAGIC;
        // // ((((ics_free_header *) themvheapptrhdft)->header)).padding_amount = 0;

        // ((ics_header *) themvheapptrhdft)->block_size = thereqcalcblksize|1;
        // ((ics_header *) themvheapptrhdft)->hid = HEADER_MAGIC;
        // ((ics_header *) themvheapptrhdft)->padding_amount = thecalcminpayloadsz - size;





        // //setup the footer
        // themvheapptrhdft+= (thereqcalcblksize-8);

        // ((ics_footer *) themvheapptrhdft)->block_size = thereqcalcblksize|1;
        // ((ics_footer *) themvheapptrhdft)->fid = FOOTER_MAGIC;


        // //splitting
        // themvheapptrhdft += 8;//after the footer
        // //need if for when already alloc
        // int remainingsz = ((numpagescalc*4096) - 16) - thereqcalcblksize;//account for prologue and epilogue
        // printf("the remaining size %d\n",remainingsz);
        
        // //allocate a blk for the empty to be placed in list
        // void * theunallocblk = themvheapptrhdft;

        // freelist_head = theunallocblk;

        // //alloc header
        // ((ics_free_header *) themvheapptrhdft)->next = NULL;
        // ((ics_free_header *) themvheapptrhdft)->prev = NULL;

        // ((((ics_free_header *) themvheapptrhdft)->header)).block_size = remainingsz|0;
        // ((((ics_free_header *) themvheapptrhdft)->header)).hid = HEADER_MAGIC;
        // ((((ics_free_header *) themvheapptrhdft)->header)).padding_amount = 0;

        // themvheapptrhdft += (remainingsz-8);

        // //alloc the footer
        // ((ics_footer *) themvheapptrhdft)->block_size = remainingsz|0;
        // ((ics_footer *) themvheapptrhdft)->fid = FOOTER_MAGIC;




        






        //test print

        // ics_header_print(thebegofheapbfinc);
        // ics_header_print(theepiloguebrkptr);
        // ics_header_print(storetheheader);
        // ics_freelist_print();

        //test insert
        // insertatheadoflist(&freelist_head,findinlistptr);

        // if(allocpagecount == 1)
        // {
        //     storenodefortest = findinlistptr;
        // }

        // //test 
        // if(allocpagecount == 2)
        // {
        //     removefromlist(&freelist_head,storenodefortest);
        // }
        


        //adding to the pagecount global
        allocpagecount += numpagescalc;

        // printf("the global page count is %d\n",allocpagecount);

        

        // return findinlistptr + 8;

    }
    else
    {
        // a block has been found in the freelist that can fit the size
        //remove it from the free list
        removefromlist(&freelist_head,findinlistptr);
    }



    //the splitting portion thereqcalcblksize

    //a pointer to the node that has been removed from list
    char * thefreeblkofflistptr = findinlistptr;

    //calc for splintering

    size_t diffbetweenfreeandalloc = (((((ics_free_header *) thefreeblkofflistptr)->header)).block_size) - (thereqcalcblksize);

    // printf("the calculated block size %ld\n",diffbetweenfreeandalloc);

    int thepaddingnum = 0;
    int theallocblocksizenum = thereqcalcblksize;

    int issplintering = 0;

    if(diffbetweenfreeandalloc < 32)
    {
        //A splinter is present include it in padding
        // printf("Splintering detected!");

        thepaddingnum = diffbetweenfreeandalloc;

        //add the difference to the allocated blocksize
        theallocblocksizenum += diffbetweenfreeandalloc;

        //set issplintering to true
        issplintering = 1;
    }

    //add internal padding in the payload
    thepaddingnum += (thecalcminpayloadsz - size);

    //the return pointer that points to the payload of alloc block
    void * thereturnpayloadptr = NULL;

    


    //splitting portion
    if(issplintering == 0)
    {
        //no splintering has been detected;

        //set up the alloc portion

        //set the header to correct fields
        ((ics_header *) thefreeblkofflistptr)->block_size = thereqcalcblksize|1;
        ((ics_header *) thefreeblkofflistptr)->hid = HEADER_MAGIC;
        ((ics_header *) thefreeblkofflistptr)->padding_amount = thepaddingnum;

        //store the payload pointer
        thereturnpayloadptr = thefreeblkofflistptr + 8;

        //move pointer to the footer of alloc block
        thefreeblkofflistptr += (thereqcalcblksize - 8);

        //set up the alloc block footer
        ((ics_footer *) thefreeblkofflistptr)->block_size = thereqcalcblksize|1;
        ((ics_footer *) thefreeblkofflistptr)->fid = FOOTER_MAGIC;

        //move pointer down to the free block to be put into list
        thefreeblkofflistptr += 8;

        //the pointer of block to be inserted if splitted
        void * theptrtothefreeblkportion = thefreeblkofflistptr;

        //set up the free block header
        ((ics_free_header *) thefreeblkofflistptr)->next = NULL;
        ((ics_free_header *) thefreeblkofflistptr)->prev = NULL;

        ((((ics_free_header *) thefreeblkofflistptr)->header)).block_size = diffbetweenfreeandalloc|0;
        ((((ics_free_header *) thefreeblkofflistptr)->header)).hid = HEADER_MAGIC;
        ((((ics_free_header *) thefreeblkofflistptr)->header)).padding_amount = 0;

        //move pointer to freeblock footer
        thefreeblkofflistptr += (diffbetweenfreeandalloc - 8);

        //set up the free block footer
        ((ics_footer *) thefreeblkofflistptr)->block_size = diffbetweenfreeandalloc|0;
        ((ics_footer *) thefreeblkofflistptr)->fid = FOOTER_MAGIC;


        //put the free block portion back into list
        insertatheadoflist(&freelist_head,theptrtothefreeblkportion);






    }
    else
    {
        //there is a splinter and it must be merged with the 

        //set the up the header to be the alloc block
        //set the header to correct fields
        //set the allocbit to 1
        ((ics_header *) thefreeblkofflistptr)->block_size = (((ics_header *) thefreeblkofflistptr)->block_size)|1;
        ((ics_header *) thefreeblkofflistptr)->hid = HEADER_MAGIC;
        ((ics_header *) thefreeblkofflistptr)->padding_amount = thepaddingnum;

        //store the payload pointer
        thereturnpayloadptr = thefreeblkofflistptr + 8;



        //move the pointer down account for footer
        thefreeblkofflistptr += (((((ics_header *) thefreeblkofflistptr)->block_size) - 1) - 8);

        //set up the alloc footer
        ((ics_footer *) thefreeblkofflistptr)->block_size = (((ics_header *) thefreeblkofflistptr)->block_size)|1;
        ((ics_footer *) thefreeblkofflistptr)->fid = FOOTER_MAGIC;




    }


    //at the end return the payload block pointer
    return thereturnpayloadptr;




    //test check if NULL

    // return NULL;
}

/*
 * Marks a dynamically allocated block as no longer in use and coalesces with 
 * adjacent free blocks (as specified by Homework Document). 
 * Adds the block to the appropriate bucket according to the block placement policy.
 *
 * @param ptr Address of dynamically allocated memory returned by the function
 * ics_malloc.
 * 
 * @return 0 upon success, -1 if error and set errno accordingly.
 */
int ics_free(void *ptr) { 

    //initial error checking

    //check pointer is between prologue and epilogue
    char * afterprologuebound = beginingofheap + 8;
    char * beforeepiloguebound = ics_get_brk() - 8;
    if(ptr < (void *)afterprologuebound || ptr > (void *)beforeepiloguebound)
    {
        // printf("error prologue/epilogue bounds check failed!\n");

        //set errno
        errno = EINVAL;
        return -1;
    }

    //checking for the givenptr header
    char * thecurrgivenheader = ptr - 8;

    ics_header * thecurheaderst = (ics_header *)thecurrgivenheader;
    if(thecurheaderst->hid != HEADER_MAGIC)
    {
        //error with header pid
        // printf("header pid is error\n");

        //set errno
        errno = EINVAL;
        return -1;
    }

    //check header is alloc
    if(isblockfreed(thecurheaderst->block_size) == 1)
    {
        //header is freed error
        // printf("error header is freed\n");

        //set errno
        errno = EINVAL;
        return -1;
    }


    //footer checks
    //the pointer to the footer
    char * thecurrgivenfooter = thecurrgivenheader + (getactualblocksize(thecurheaderst->block_size) - 8);

    ics_footer * thecurfooterst = (ics_footer *)thecurrgivenfooter;

    //check footer fid
    if(thecurfooterst->fid != FOOTER_MAGIC)
    {
        //error with footer pid
        // printf("footer pid is error\n");

        //set errno
        errno = EINVAL;
        return -1;
    }

    //check footer is alloc
    if(isblockfreed(thecurfooterst->block_size) == 1)
    {
        //footer is freed error
        // printf("error footer is freed\n");

        //set errno
        errno = EINVAL;
        return -1;
    }

    //check matching blocksizes between footer and header
    if(getactualblocksize(thecurheaderst->block_size) != getactualblocksize(thecurfooterst->block_size))
    {
        //error the block sizes of footer and header dont match
        // printf("mis match footer and header blk sizes\n");

        //set errno
        errno = EINVAL;
        return -1;
    }


    //after error checks

    // //test free the block and put back into freelist
    //set header to free
    thecurheaderst->block_size = (thecurheaderst->block_size) & -2;
    //set footer to free
    thecurfooterst->block_size = (thecurfooterst->block_size) & -2;

    // //add back to the freelist
    // insertatheadoflist(&freelist_head,thecurrgivenheader);

    //asize var to store currentsize for coalsecs
    uint64_t totalblksize = thecurheaderst->block_size;

    //coalsesc in reverse
    //check block after current
    char * theblkaftercurrentheaderptr = thecurrgivenfooter + 8;

    //check if it is free 
    if(isblockfreed(((ics_header *) theblkaftercurrentheaderptr)->block_size) == 1)
    {
        // printf("can reverse coaleses \n");
        //the blk after current is free can reverse coalse

        //add to the total size
        totalblksize += ((ics_header *) theblkaftercurrentheaderptr)->block_size;

        //remove the blk after current from the list
        removefromlist(&freelist_head,theblkaftercurrentheaderptr);

        //move the currentfooterptr to thefooter of theblk after
        thecurrgivenfooter += ((ics_header *) theblkaftercurrentheaderptr)->block_size;

        //update the current header and footer to new size
        ((ics_header *) thecurrgivenheader)->block_size = totalblksize;

        //update the current footer
        ((ics_footer *) thecurrgivenfooter)->block_size = totalblksize;
        

        
    }

    // //forward coalscing
    // char * theblkbeforecurrentfooterptr = thecurrgivenheader - 8; //set to the footerblk before current header

    // //check if it is free
    // if(isblockfreed(((ics_header *) theblkbeforecurrentfooterptr)->block_size) == 1)
    // {
    //     //coalse forware with blk before the current
    //     printf("can coalse forward with block before current\n");

    //     //add to the total size
    //     totalblksize += ((ics_header *) theblkbeforecurrentfooterptr)->block_size;

    //     //move the current header to point at the prev blk's header
    //     thecurrgivenheader -= ((ics_header *) theblkbeforecurrentfooterptr)->block_size;

    //     //remove the blk before from the free list
    //     removefromlist(&freelist_head,thecurrgivenheader);

    //     //update the current header and footer to the new size
    //     //update the current header and footer to new size
    //     ((ics_header *) thecurrgivenheader)->block_size = totalblksize;

    //     //update the current footer
    //     ((ics_footer *) thecurrgivenfooter)->block_size = totalblksize;
    // }


    //at the end add to the freelist the current header
    insertatheadoflist(&freelist_head,thecurrgivenheader);

    //after completion of free successfully return 0

    return 0;

    
}

/********************** EXTRA CREDIT ***************************************/

/*
 * Resizes the dynamically allocated memory, pointed to by ptr, to at least size 
 * bytes. See Homework Document for specific description.
 *
 * @param ptr Address of the previously allocated memory region.
 * @param size The minimum size to resize the allocated memory to.
 * @return If successful, the pointer to the block of allocated memory is
 * returned. Else, NULL is returned and errno is set appropriately.
 *
 * If there is no memory available ics_malloc will set errno to ENOMEM. 
 *
 * If ics_realloc is called with an invalid pointer, set errno to EINVAL. See ics_free
 * for more details.
 *
 * If ics_realloc is called with a valid pointer and a size of 0, the allocated     
 * block is free'd and return NULL.
 */
void *ics_realloc(void *ptr, size_t size) {
    return NULL;
}
