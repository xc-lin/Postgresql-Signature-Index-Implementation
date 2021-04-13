// Test Bits ADT

#include <stdio.h>
#include "defs.h"
#include "reln.h"
#include "tuple.h"
#include "bits.h"

int main(int argc, char **argv) {
    Bits b = newBits(60);
    Bits b2 = newBits(60);
    printf("t=0: ");
    showBits(b);
    printf("\n");
    setBit(b, 5);
    printf("t=1: ");
    showBits(b);
    printf("\n");
    setBit(b, 0);
    setBit(b, 50);
    setBit(b, 59);
    printf("t=2: ");
    showBits(b);
    printf("\n");

    setBit(b2, 1);
    setBit(b2, 0);
    //-------or
    printf("b  : ");
    showBits(b);
    printf("\n");
    printf("b2 : ");
    showBits(b2);
    printf("\n");
    orBits(b, b2);
    printf("or : ");
    showBits(b);
    printf("\n");

    //-------and
    printf("b  : ");
    showBits(b);
    printf("\n");
    printf("b2 : ");
    showBits(b2);
    printf("\n");
    andBits(b, b2);
    printf("and: ");
    showBits(b);
    printf("\n");

    //-------subset
    setBit(b, 0);
    setBit(b, 50);
    setBit(b, 59);
    setBit(b2, 50);
    setBit(b2, 0);
    isSubset(b2, b);
    printf("subset: %d", isSubset(b2, b));
    printf("\n");

    if (bitIsSet(b, 5)) printf("Bit 5 is set\n");
    if (bitIsSet(b, 10)) printf("Bit 10 is set\n");
    printf("============\n");
    setAllBits(b);
    printf("t=3: ");
    showBits(b);
    printf("\n");
    unsetAllBits(b);
    printf("t=4: ");
    showBits(b);
    printf("\n");
    setAllBits(b);
    unsetBit(b, 40);
    printf("t=5: ");
    showBits(b);
    printf("\n");
    if (bitIsSet(b, 20)) printf("Bit 20 is set\n");
    if (bitIsSet(b, 40)) printf("Bit 40 is set\n");
    if (bitIsSet(b, 50)) printf("Bit 50 is set\n");
    setBit(b, 59);


    Bits b3 = newBits(32);
    setBit(b3,1);
    setBit(b3,4);
    setBit(b3,6);
    setBit(b3,9);
    setBit(b3,10);
    showBits(b3);
    printf("\n");
    shiftBits(b3,5);
    showBits(b3);
    printf("\n");

    shiftBits(b3,-7);
    showBits(b3);
    printf("\n");

    freeBits(b);
    return 0;
}
