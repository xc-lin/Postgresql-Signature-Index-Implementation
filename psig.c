// psig.c ... functions on page signatures (psig's)
// part of signature indexed files
// Written by John Shepherd, March 2019

#include "defs.h"
#include "reln.h"
#include "query.h"
#include "psig.h"
#include "hash.h"

Bits makePageSig(Reln r, Tuple t) {
    assert(r != NULL && t != NULL);
    //TODO
    int m = psigBits(r);
    int k = codeBits(r);
    int bitWidth;
    int bitWidth_i;
    int bitWidth_1;
    char **attributes = tupleVals(r, t);
    int n = nAttrs(r);
    Bits finalBits = newBits(m);
    switch (r->params.sigtype) {
        case 's':
            for (int i = 0; i < n; i++) {
                if (attributes[i][0] == '?') continue;
                int nbits = 0; // count of set
                Bits bits = newBits(m);
                srandom(hash_any(attributes[i], strlen(attributes[i])));
                while (nbits < k) {
                    int rd = random() % m;
                    setBit(bits, rd);
                    nbits++;
                }
                orBits(finalBits, bits);
            }
            break;
        case 'c':
            bitWidth_i = m / n;
            bitWidth_1 = m / n + m % nAttrs(r);
            for (int i = 0; i < n; i++) {
                if (i == 0) {
                    bitWidth = bitWidth_1;
                } else {
                    bitWidth = bitWidth_i;
                }

                k = bitWidth / 2 / maxTupsPP(r);
                if (attributes[i][0] == '?') continue;

                int nbits = 0; // count of set
                Bits bits = newBits(m);
                srandom(hash_any(attributes[i], strlen(attributes[i])));
                while (nbits < k) {
                    int rd = random() % bitWidth;
                    setBit(bits, rd);

                    // printf("%d\n",rd);
                    nbits++;
                }
                /*showBits(bits);
                printf("\n");*/

                if (i > 0) {
                    shiftBits(bits, (i - 1) * bitWidth_i + bitWidth_1);
                }

                orBits(finalBits, bits);


            }
            /*   showBits(finalBits);
               printf("\n");*/
            break;
    }

    return finalBits;
}

void findPagesUsingPageSigs(Query q) {
    assert(q != NULL);
    //TODO
    Bits querySig = makePageSig(q->rel, q->qstring);
    int sigCount = 0;
    int pageNum = nPsigPages(q->rel);
    for (int i = 0; i < pageNum; ++i) {
        Page p = getPage(psigFile(q->rel), i);
        for (int j = 0; j < pageNitems(p); j++) {
            int size = psigBits(q->rel);
            Bits psig = newBits(size);
            getBits(p, j , psig);
            /*showBits(querySig);
            printf(":query\n");
            showBits(psig);
            printf(":psig:\n")*/;
            if (isSubset(querySig, psig)) {
                // printf("==============\n");
                setBit(q->pages, sigCount);
            }
            sigCount++;

        }
        free(p);
    }
    q->nsigpages = pageNum;
    q->nsigs = sigCount;
    // The printf below is primarily for debugging
    // Remove it before submitting this function
   /* printf("Matched Pages:");
    showBits(q->pages);

    putchar('\n');*/
}

