#include "debug.h"
#include "helpers.h"
#include "icsmm.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>

// size_t calcminpayloadsize(size_t size)
// {
//     //
//     int intdiv = (size/16); // the ceil funct

//     if(size%16 != 0)
//     {
//         intdiv++;
//     }

//     printf("the intdiv %d\n",intdiv);

    

//     return 16*intdiv;
// }

// int calcpagenumbers(int ablocksize,int pagesallocedalready)
// {
//     if(pagesallocedalready == 0)
//     {

//         //need 4064 for first page 4096 for next subsquent
//         if(ablocksize <= 4064)
//         {
//             return 1;
//         }
//         else
//         {
//             int thepagescalc = 1;
//             //subtract 
//             int payloadwithoutfpage  = ablocksize-4064;

//             if(payloadwithoutfpage % 4096 == 0)
//             {
//                 thepagescalc += payloadwithoutfpage/4096;
//             }
//             else
//             {
//                 thepagescalc += payloadwithoutfpage/4096 + 1;
//             }

//             printf("int page calc div %d\n",thepagescalc);

//             return thepagescalc;



//         }
        

//     }
//     else
//     {
//         //for the case where a page is already allocated
//         //offset by 8 byte for the footer and epilogue at end and header is not included
//         ablocksize -= 8;

//         int pagecount = 0;
        

//         while(ablocksize > 0)
//         {
//             pagecount++;
//             ablocksize -= 4096;
//         }

//         return pagecount;

//     }


    
// }

int main()
{
    printf("hello this is a test\n");

    //test calculations

    // int retvv = calcminpayloadsize(8161);

    // printf("the return size %d\n",retvv);

    // int thepagescaclc = calcpagenumbers(retvv + 16,0);

    // printf("the page size is %d\n",thepagescaclc);

    //test
    ics_mem_init();

    void * theretptr = ics_malloc(4032);
    // void * theretptr2 = ics_malloc(32);
    // void * theretptr3 = ics_malloc(64);
    // // theretptr = ics_malloc(1);
    // ics_payload_print(theretptr);
    ics_freelist_print();

    printf("after the first mallco with 32b free block at end\n");
    void * themal2 = ics_malloc(32);

    printf("the debugging print statements\n");
    ics_freelist_print();
    printf("the payload debugging\n");
    ics_payload_print(themal2);

    //test the git change 1223

    

    // theretptr = ics_malloc(10000);
    // ics_payload_print(theretptr);
    // theretptr = ics_malloc(30000);

    // perror("The print error");

    // theretptr = ics_malloc(1);
    // theretptr = ics_malloc(1);
    // char * thestrptr = ics_malloc(16);

    // // strcpy(thestrptr,"hellohellohellohellwjsbjdsbjsabjdsabjasdjb");

    // // printf("this is in the malloc str: %s\n",thestrptr);

    // thestrptr -= 8;

    // ics_header * theheadder  = theretptr - 8;

    // printf("test bit masking %d\n", getactualblocksize(theheadder->block_size));

    // ics_payload_print(thestrptr);
    // ics_freelist_print();
    // // printf("%d \n",2|1);


    // //test free
    // printf("testing the free section\n");
    // char * anullblktest = (char *)freelist_head + 8;
    // ics_free(theretptr);

    // ics_free(theretptr3);

    // ics_free(theretptr2);

    // //print afterwards
    // ics_freelist_print();

    

    // printf("bit shift test %d\n",((15>>4)<<4) == 15);

    //the grade scope tests
    // printf("request 1 =================================\n");
    // void *ptr0 = ics_malloc(5000);
    // printf("request 2 =================================\n");
    // void *ptr1 = ics_malloc(5000);
    // printf("request 3 =================================\n");
    // void *ptr2 = ics_malloc(5000);
    // printf("request 4 =================================\n");
    // void *ptr3 = ics_malloc(5000);
    // printf("request 5 =================================\n");
    // void *ptr4 = ics_malloc(5000);

    ics_mem_fini();
}