// tsig.c ... functions on Tuple Signatures (tsig's)
// part of signature indexed files
// Written by John Shepherd, March 2019

#include <unistd.h>
#include <string.h>
#include "defs.h"
#include "tsig.h"
#include "reln.h"
#include "hash.h"
#include "bits.h"

// make a tuple signature


Bits makeTupleSig(Reln r, Tuple t) {
    assert(r != NULL && t != NULL);
    //TODO
    int m = tsigBits(r);
    int k = codeBits(r);
    int bitWidth;
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
            bitWidth = m / n;
            int temp = bitWidth;
            for (int i = 0; i < n; i++) {
                if (i == 0) {
                    bitWidth += m % nAttrs(r);
                } else {
                    bitWidth = temp;
                }
                //k = bitWidth / 2;
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
                /* showBits(bits);
                 printf("\n");*/
                shiftBits(bits, i * bitWidth);

                orBits(finalBits, bits);


            }
            /*showBits(finalBits);
            printf("\n");*/
            break;
    }

    return finalBits;
}

// find "matching" pages using tuple signatures

void findPagesUsingTupSigs(Query q) {
    assert(q != NULL);
    Bits querySig = makeTupleSig(q->rel, q->qstring);
    int sigCount = 0;
    int pageNum = nTsigPages(q->rel);
    for (int i = 0; i < pageNum; ++i) {
        Page p = getPage(tsigFile(q->rel), i);
        for (int j = 0; j < pageNitems(p); j++) {
            int size = tsigBits(q->rel);
            Bits tsig = newBits(size);
            getBits(p, j, tsig);
            //TODO
            if (isSubset(querySig, tsig)) {
                setBit(q->pages, sigCount / maxTupsPP(q->rel));
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

