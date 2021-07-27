// bsig.c ... functions on Tuple Signatures (bsig's)
// part of signature indexed files
// Written by John Shepherd, March 2019

#include "defs.h"
#include "reln.h"
#include "query.h"
#include "bsig.h"
#include "psig.h"

void findPagesUsingBitSlices(Query q) {
    assert(q != NULL);
    int pageNum = 0;
    int sigCount = 0;
    int preBsigPid = -1;
    Page p;
    Reln r = q->rel;
    Bool pageIsAllocated=FALSE;
    Bits querySig = makePageSig(r, q->qstring);
    setAllBits(q->pages);


    for (int i = 0; i < psigBits(r); ++i) {
        if (bitIsSet(querySig, i)) {
            int bsigPid = i / maxBsigsPP(r);
            if (preBsigPid != bsigPid) {
                pageNum++;
                if (pageIsAllocated){
                    free(p);
                }
                p = getPage(r->bsigf, bsigPid);
                pageIsAllocated=TRUE;
            }
            preBsigPid = bsigPid;
            sigCount++;
            Bits bitSlices = newBits(bsigBits(r));
            getBits(p, i % maxBsigsPP(r), bitSlices);
            for (int j = 0; j < nPages(r); ++j) {
                if(!bitIsSet(bitSlices,j)){
                    unsetBit(q->pages,j);
                }
            }
            freeBits(bitSlices);
        }
    }
    freeBits(querySig);
    free(p);
    q->nsigpages = pageNum;
    q->nsigs = sigCount;
    // The printf below is primarily for debugging
    // Remove it before submitting this function
    /*printf("Matched Pages:");
    showBits(q->pages);
    printf("\n");
*/

    //setAllBits(q->pages); // remove this
}


