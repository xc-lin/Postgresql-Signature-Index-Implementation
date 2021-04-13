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
    Page p;
    Reln r = q->rel;
    Bits querySig = makePageSig(r, q->qstring);
    Bits Pages = newBits(bsigBits(r));
    setAllBits(Pages);
    int preBsigPid = -1;

    for (int i = 0; i < psigBits(r); ++i) {
        if (bitIsSet(querySig, i)) {
            int bsigPid = i / maxBsigsPP(r);
            if (preBsigPid != bsigPid) {
                pageNum++;
                p = getPage(r->bsigf, bsigPid);
            }
            preBsigPid = bsigPid;
            sigCount++;

            Bits bitSlices = newBits(bsigBits(r));
            getBits(p, i % maxBsigsPP(r), bitSlices);
            andBits(Pages, bitSlices);
        }
    }
    free(p);
    for (int i = 0; i < nPages(r); ++i) {
        if (bitIsSet(Pages, i)) {
            setBit(q->pages, i);
        }
    }

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


