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
#define V2_NORM(a,len) ((Vector2){(a).x*len/(abs(a.x)+abs(a.y)),(a).y*len/(abs(a.x)+abs(a.y))})

Rect resizeRect(Rect r, int magnitudeDifference);

// Returns int in [min, max)
int randRange(int min, int max);

// Returns -1, 0, or 1 based on sign
int sign(int v);

#endif