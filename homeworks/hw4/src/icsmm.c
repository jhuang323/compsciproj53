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
    // return NULL;

    //test getting more than 6 pages
    void * thecurrbrk = ics_get_brk();

    printf("the current breakpoint %p\n",thecurrbrk);

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
            printf("warning the heap inc error\n");
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
        
        if(allocpagecount == 0)
        {
            //first time move it past the prologue
            themvheapptr += 8;
        }
        else
        {
            themvheapptr -= 8;
        }

        //store the freeblock header
        findinlistptr = themvheapptr;

        //set up the free block header
        //calc size
        int thecalcsizefreeblk = 0;
        if(allocpagecount == 0)
        {
            thecalcsizefreeblk = 4096 * numpagescalc;

            thecalcsizefreeblk -= 16; //for the prologue and epilogue

        }
        else
        {
            thecalcsizefreeblk = 4096 * numpagescalc;//for the footer a the end
            
        }

        // printf("the calc blk size %ld\n",thereqcalcblksize);

        //set up the header
        //alloc header
        ((ics_free_header *) themvheapptr)->next = NULL;
        ((ics_free_header *) themvheapptr)->prev = NULL;

        ((((ics_free_header *) themvheapptr)->header)).block_size = thecalcsizefreeblk|0;
        ((((ics_free_header *) themvheapptr)->header)).hid = HEADER_MAGIC;
        ((((ics_free_header *) themvheapptr)->header)).padding_amount = 0;



        

        //setup the epilogue
        char * theepiloguebrkptr = ics_get_brk();
        printf("the brak is %p\n",ics_get_brk());
        theepiloguebrkptr -= 8;
        ((ics_header *) theepiloguebrkptr)->block_size = 1;
        ((ics_header *) theepiloguebrkptr)->hid = EPILOGUE_MAGIC;
        ((ics_header *) theepiloguebrkptr)->padding_amount = 0;

        //set up the footer
        theepiloguebrkptr -= 8;
        ((ics_footer *) theepiloguebrkptr)->block_size = thecalcsizefreeblk|0;
        ((ics_footer *) theepiloguebrkptr)->fid = FOOTER_MAGIC;

        printf("the footerlocation %p\n",theepiloguebrkptr);




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


        //adding to the pagecount global
        allocpagecount += numpagescalc;

        printf("the global page count is %d\n",allocpagecount);

        return findinlistptr + 8;

    }

    //test check if NULL

    return NULL;
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
    return -99999;
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
