#include "mgpmath.h"
#include "stdlib.h"

int Vector2Length(Vector2 v, int fixedBits) {
    int x2 = (v.x * v.x);
    int y2 = (v.y * v.y);
    // mgba_printf("X^2: %x", x2);
    // mgba_printf("Y^2: %x", y2);
    return fixedsqrt(x2+y2,2*fixedBits)>>fixedBits;
}

Vector2 Vector2Normalize(Vector2 v, int fixedBits) {
    int len = Vector2Length(v,fixedBits);
    // mgba_printf("vector len: %x", len);
    v.x = (v.x<<fixedBits)/len;
    v.y = (v.y<<fixedBits)/len;
    return v;
}

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

unsigned int fixedsqrt(unsigned int x, unsigned int fixedBits) {
    return usqrt(x)>>(16-fixedBits/2);
}

// Modified from https://stackoverflow.com/questions/1100090/looking-for-an-efficient-integer-square-root-algorithm-for-arm-thumb2
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