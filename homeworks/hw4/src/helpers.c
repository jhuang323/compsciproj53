#include "helpers.h"
#include "debug.h"

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
        if(ablocksize <= 4064)
        {
            return 1;
        }
        else
        {
            int thepagescalc = 1;
            //subtract 
            int payloadwithoutfpage  = ablocksize-4064;

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

    
    

    //satisft error
    return NULL;
}
