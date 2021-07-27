// query.c ... query scan functions
// part of signature indexed files
// Manage creating and using Query objects
// Written by John Shepherd, March 2019

#include "defs.h"
#include "query.h"
#include "reln.h"
#include "tuple.h"
#include "bits.h"
#include "tsig.h"
#include "psig.h"
#include "bsig.h"

// check whether a query is valid for a relation
// e.g. same number of attributes

int checkQuery(Reln r, char *q) {
    if (*q == '\0') return 0;
    char *c;
    int nattr = 1;
    for (c = q; *c != '\0'; c++)
        if (*c == ',') nattr++;
    return (nattr == nAttrs(r));
}

// take a query string (e.g. "1234,?,abc,?")
// set up a QueryRep object for the scan

Query startQuery(Reln r, char *q, char sigs) {
    Query new = malloc(sizeof(QueryRep));
    assert(new != NULL);
    if (!checkQuery(r, q)) return NULL;
    new->rel = r;
    new->qstring = q;
    new->nsigs = new->nsigpages = 0;
    new->ntuples = new->ntuppages = new->nfalse = 0;
    new->pages = newBits(nPages(r));
    switch (sigs) {
        case 't':
            findPagesUsingTupSigs(new);
            break;
        case 'p':
            findPagesUsingPageSigs(new);
            break;
        case 'b':
            findPagesUsingBitSlices(new);
            break;
        default:
            setAllBits(new->pages);
            break;
    }
    new->curpage = 0;
    return new;
}

// scan through selected pages (q->pages)
// search for matching tuples and show each
// accumulate query stats

void scanAndDisplayMatchingTuples(Query q) {
    assert(q != NULL);
    //TODO
    Page p;
    Tuple t;
    int examinedTupCount = 0;
    int falseMatchPageCount = 0;
    int dataPageCount = 0;

    for (int pid = 0; pid < nPages(q->rel); ++pid) {
        q->curpage = pid;
        Bool flag = TRUE;
        if (bitIsSet(q->pages, pid)) {
            //indicate the page is selected
            p = getPage(dataFile(q->rel), pid);
            dataPageCount++;
            for (int z = 0; z < pageNitems(p); z++) {
                q->curtup = z;
                t = getTupleFromPage(q->rel, p, z);
                examinedTupCount++;
                if (tupleMatch(q->rel, q->qstring, t)) {
                    flag = FALSE;
                    showTuple(q->rel, t);
                    free(t);
                }
            }
            free(p);
            if (flag) falseMatchPageCount++;
        }
    }
    q->nfalse = falseMatchPageCount;
    q->ntuples = examinedTupCount;
    q->ntuppages = dataPageCount;
}

// print statistics on query

void queryStats(Query q) {
    printf("# sig pages read:    %d\n", q->nsigpages);
    printf("# signatures read:   %d\n", q->nsigs);
    printf("# data pages read:   %d\n", q->ntuppages);
    printf("# tuples examined:   %d\n", q->ntuples);
    printf("# false match pages: %d\n", q->nfalse);
}

// clean up a QueryRep object and associated data

void closeQuery(Query q) {
    free(q->pages);
    free(q);
}

