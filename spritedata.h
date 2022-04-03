#ifndef SPRITEDATA_H
#define SPRITEDATA_H

// Sprite Attribute Struct
typedef struct {
    unsigned short attr0;
    unsigned short attr1;
    unsigned short attr2;
    unsigned short fill;
} OBJ_ATTR;

// Object Attribute Memory
#define OAM ((OBJ_ATTR*)(0x7000000))
extern OBJ_ATTR shadowOAM[];

// Attribute 0
#define ATTR0_REGULAR      (0<<8)  // Normal Rendering
#define ATTR0_AFFINE       (1<<8)  // Affine Rendering
#define ATTR0_HIDE         (2<<8)  // No Rendering (Hidden)
#define ATTR0_DOUBLEAFFINE (3<<8)  // Double Affine Rendering
#define ATTR0_NOBLEND      (0<<10) // Disable Alpha Blending
#define ATTR0_BLEND        (1<<10) // Enable Alpha Blending
#define ATTR0_WINDOW       (2<<10) // Object Window Mode
#define ATTR0_MOSAIC       (1<<12) // Enable Mosaic Effect
#define ATTR0_4BPP         (0<<13)
#define ATTR0_8BPP         (1<<13)
#define ATTR0_SQUARE       (0<<14)
#define ATTR0_WIDE         (1<<14) //        |  TINY  | SMALL  | MEDIUM | LARGE  |
#define ATTR0_TALL         (2<<14) // --------------------------------------------
                                   // SQUARE |  8x8   | 16x16  | 32x32  | 64x64  |
// Attribute 1                     // --------------------------------------------
#define ATTR1_HFLIP  (1<<12)       //  WIDE  |  16x8  | 32x8   | 32x16  | 64x32  |
#define ATTR1_VFLIP  (1<<13)       // --------------------------------------------
#define ATTR1_TINY   (0<<14)       //  TALL  |  8x16  | 8x32   | 16x32  | 32x64  |
#define ATTR1_SMALL  (1<<14)       // --------------------------------------------
#define ATTR1_MEDIUM (2<<14)
#define ATTR1_LARGE  (3<<14)

// Attribute 2
#define ATTR2_TILEID(col, row) ((row)*32+(col))
#define ATTR2_PRIORITY(num)    ((num)<<10)
#define ATTR2_PALROW(row)      ((row)<<12)

void hideSprites(void);

#define RESERVED_OAM_SIZE 0

void initSpriteMemory(void);
OBJ_ATTR *spriteAlloc(void);
void spriteFree(OBJ_ATTR* sprite);

void updateSprites(void);

#endif