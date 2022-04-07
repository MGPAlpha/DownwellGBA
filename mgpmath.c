#include "mgpmath.h"
#include "stdlib.h"

Rect resizeRect(Rect r, int magnitudeDifference) {
    r.pos.x <<= magnitudeDifference;
    r.pos.y <<= magnitudeDifference;
    r.size.x <<= magnitudeDifference;
    r.size.y <<= magnitudeDifference;
    return r;
}

int randRange(int min, int max) {
    int randVal = (int)( rand() % (max-min) );
    return randVal + min;
}

int sign(int v) {
    return (v > 0) ? 1 : (v < 0 ? -1 : 0);
}

// Modified from Turkowski & Apple Computer (1994) https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.178.3957&rep=rep1&type=pdf#:~:text=Often%2C%20a%20fixed%2Dpoint%20algorithm,and%20evaluating%20in%20floating%2Dpoint.
int FFracSqrt(int x) {
    register unsigned int root, remHi, remLo, testDiv, count;
    root = 0; /* Clear root */
    remHi = 0; /* Clear high part of partial remainder */
    remLo = x; /* Get argument into low part of partial remainder */
    count = 30; /* Load loop counter */
    do {
        remHi = (remHi<<2) | (remLo>>30); remLo <<=2; /* get 2 bits of arg */
        root <<= 1; /* Get ready for the next bit in the root */
        testDiv = (root << 1) + 1; /* Test radical */
        if (remHi >= testDiv) {
            remHi -= testDiv;
            root++;
        }
    } while (count-- != 0);
    return(root);
}

unsigned int fixedsqrt(unsigned int x, unsigned int fixedBits) {
    return usqrt(x)>>(16-fixedBits/2);
}

unsigned int usqrt(unsigned int x)
{
      unsigned int a = 0L;                   /* accumulator      */
      unsigned int r = 0L;                   /* remainder        */
      unsigned int e = 0L;                   /* trial product    */

      int i;

      for (i = 0; i < 32; i++)   /* NOTE 1 */
      {
            r = (r << 2) + (x>>30); x <<= 2; /* NOTE 2 */
            a <<= 1;
            e = (a << 1) + 1;
            if (r >= e)
            {
                  r -= e;
                  a++;
            }
      }
      return a;
}