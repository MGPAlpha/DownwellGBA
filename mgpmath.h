#ifndef MGP_MATH
#define MGP_MATH

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct Vector2 {
    int x;
    int y;
} Vector2;

typedef struct rect {
    Vector2 pos;
    Vector2 size;
} Rect;
typedef Rect Transform;

#define V2_ADD(a,b) ((Vector2){(a).x+(b).x,(a).y+(b).y})
#define V2_SUB(a,b) ((Vector2){(a).x-(b).x,(a).y-(b).y})
#define V2_MUL(a,b) ((Vector2){(a).x*(b),(a).y*(b)})
#define V2_DIV(a,b) ((Vector2){(a).x/(b),(a).y/(b)})


int Vector2Length(Vector2 v, int fixedBits);
Vector2 Vector2Normalize(Vector2 v, int fixedBits);

Rect resizeRect(Rect r, int magnitudeDifference);

// Returns int in [min, max)
int randRange(int min, int max);

// Returns -1, 0, or 1 based on sign
int sign(int v);

unsigned int fixedsqrt(unsigned int x, unsigned int fixedBits);
unsigned int usqrt(unsigned int x);

#endif