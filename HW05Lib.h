/*
 * Based on a modified version of HW04Lib
 */

#ifndef HW05_LIB_H
#define HW05_LIB_H

#include "mgpmath.h"

// Typedefs & Structs
typedef unsigned short u16;
typedef u16 color;
typedef struct rect {
    Vector2 pos;
    Vector2 size;
} Rect;
typedef Rect Transform;

// Common Macros
#define OFFSET(c, r, rowlen) ((r)*(rowlen)+(c))

// ================================= DISPLAY ==================================

// Display Control Register
#define REG_DISPCTL (*(unsigned short *)0x4000000)
#define MODE0 0
#define MODE3 3
#define MODE4 4
#define DISP_BACKBUFFER (1<<4)
#define BG0_ENABLE      (1<<8)
#define BG1_ENABLE      (1<<9)
#define BG2_ENABLE      (1<<10)
#define BG3_ENABLE      (1<<11)
#define SPRITE_ENABLE   (1<<12)
#define SPRITE_MODE_2D  (0<<6)
#define SPRITE_MODE_1D  (1<<6)

#define REG_BG0CNT (*(volatile unsigned short*)0x4000008)
#define REG_BG1CNT (*(volatile unsigned short*)0x400000A)
#define REG_BG2CNT (*(volatile unsigned short*)0x400000C)
#define REG_BG3CNT (*(volatile unsigned short*)0x400000E)

#define BG_CHARBLOCK(num)   ((num)<<2)
#define BG_SCREENBLOCK(num) ((num)<<8)
#define BG_4BPP             (0<<7)
#define BG_8BPP             (1<<7)
#define BG_SIZE_SMALL       (0<<14)  // 32x32 tiles
#define BG_SIZE_WIDE        (1<<14)  // 64x32 tiles
#define BG_SIZE_TALL        (2<<14)  // 32x64 tiles
#define BG_SIZE_LARGE       (3<<14)  // 64x64 tiles

// Background Offset Registers
#define REG_BG0HOFF (*(volatile unsigned short *)0x04000010)
#define REG_BG0VOFF (*(volatile unsigned short *)0x04000012)

#define REG_BG1HOFF (*(volatile unsigned short *)0x04000014)
#define REG_BG1VOFF (*(volatile unsigned short *)0x04000016)

#define REG_BG2HOFF (*(volatile unsigned short *)0x04000018)
#define REG_BG2VOFF (*(volatile unsigned short *)0x0400001A)

#define REG_BG3HOFF (*(volatile unsigned short *)0x0400001C)
#define REG_BG3VOFF (*(volatile unsigned short *)0x0400001E)

// Display Status Registers
#define SCANLINECOUNTER (*(volatile unsigned short *)0x4000006)

// Display Constants
#define SCREENHEIGHT 160
#define SCREENWIDTH 240

// Video Buffer
extern unsigned short *videoBuffer;
#define FRONTBUFFER ((unsigned short *)0x6000000)
#define BACKBUFFER ((unsigned short *)0x600A000)

// Palette
#define PALETTE       ((unsigned short *)0x5000000)
#define SPRITEPALETTE ((unsigned short *)0x5000200)

// Color
#define COLOR(r,g,b) ((r) | (g)<<5 | (b)<<10)
#define BLACK 0
#define WHITE COLOR(31,31,31)
#define GRAY COLOR(15,15,15)
#define RED COLOR(31,0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define CYAN COLOR(0,31,31)
#define MAGENTA COLOR(31,0,31)
#define YELLOW COLOR(31,31,0)
// Custom Colors
#define PORTAGE COLOR(18,19,27)
#define CADILLAC COLOR(19,9,13)
#define ROGUE COLOR(22,6,13)
#define VIORED COLOR(28,8,16)
#define HOTPINK COLOR(31,13,21)
#define LAVPINK COLOR(31,20,26)
#define TURQUOISE COLOR(5,26,25)
#define PERSIAN COLOR(0,21,20)
#define TEAL COLOR(0,16,16)

typedef struct {
	u16 tileimg[8192];
} charblock;
#define CHARBLOCK ((charblock *)0x6000000)

typedef struct {
	u16 tilemap[1024];
} screenblock;
#define SCREENBLOCK ((screenblock *)0x6000000)

// Mode 3 Drawing Functions
void setPixel3(Vector2 pos, unsigned short color);
void drawRect3(Rect rect, volatile unsigned short color);
void fillScreen3(volatile unsigned short color);
void drawImage3(Vector2 pos, Vector2 size, const unsigned short *image);
void drawFullscreenImage3(const unsigned short *image);

// Mode 4 Drawing Functions
void setPixel4(Vector2 pos, unsigned char colorIndex);
void drawRect4(Rect rect, volatile unsigned char colorIndex);
void fillScreen4(volatile unsigned char colorIndex);
void drawImage4(Vector2 pos, Vector2 size, const unsigned short *image);
void drawFullscreenImage4(const unsigned short *image);

// Miscellaneous Drawing Functions
void waitForVBlank();
void flipPage();


// ================================== INPUT ===================================

// Button Register
#define BUTTONS (*(volatile unsigned short *)0x04000130)

// Button Masks
#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

// Variables for Button Macros
extern unsigned short oldButtons;
extern unsigned short buttons;

// Button Macros
#define BUTTON_HELD(key)  (~(BUTTONS) & (key))
#define BUTTON_PRESSED(key) (!(~(oldButtons)&(key)) && (~buttons & (key)))


// =================================== DMA ====================================

// DMA Struct
typedef volatile struct {
    volatile const void *src;
    volatile void *dst;
    unsigned int cnt;
} DMA;

// First DMA Register Address
extern DMA *dma;

// Destination Adjustment
#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

// Source Adjustment
#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

// VBlank and HBlank Repeats
#define DMA_REPEAT (1 << 25)

// Chunk Size
#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

// Timing Mode
#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

// Interrupts
#define DMA_IRQ (1 << 30)

// Begin DMA
#define DMA_ON (1 << 31)

// DMA Functions
void DMANow(int channel, volatile const void *src, volatile void *dst, unsigned int cnt);

// =================================== SOUND ====================================

#define PROCESSOR_CYCLES_PER_SECOND (16777216)
#define VBLANK_FREQ (59.727)

// register definitions
#define REG_SOUNDCNT_L        *(volatile u16*)0x04000080
#define REG_SOUNDCNT_H        *(volatile u16*)0x04000082
#define REG_SOUNDCNT_X        *(volatile u16*)0x04000084
#define REG_SOUNDBIAS         *(volatile u16*)0x04000086


// flags
#define SND_ENABLED           (1<<7)
#define SND_OUTPUT_RATIO_25   0
#define SND_OUTPUT_RATIO_50   (1<<0)
#define SND_OUTPUT_RATIO_100  (1<<1)

// ============================== ANALOG SOUND ================================

#define REG_SND1SWEEP     *(volatile u16*)0x04000060
#define REG_SND1CNT       *(volatile u16*)0x04000062
#define REG_SND1FREQ      *(volatile u16*)0x04000064

#define REG_SND2CNT       *(volatile u16*)0x04000068
#define REG_SND2FREQ      *(volatile u16*)0x0400006C

#define REG_SND3SEL       *(volatile u16*)0x04000070
#define REG_SND3CNT       *(volatile u16*)0x04000072
#define REG_SND3FREQ      *(volatile u16*)0x04000074

#define REG_SND4CNT       *(volatile u16*)0x04000078
#define REG_SND4FREQ      *(volatile u16*)0x0400007C

// Channel 3 Wave Pattern RAM (2 banks!!)
#define REG_SND3_WAV      *(volatile u16*)0x04000090

#define DMG_SND1_LEFT         (1 << 8)
#define DMG_SND2_LEFT         (1 << 9)
#define DMG_SND3_LEFT         (1 << 10)
#define DMG_SND4_LEFT         (1 << 11)

#define DMG_SND1_RIGHT        (1 << 12)
#define DMG_SND2_RIGHT        (1 << 13)
#define DMG_SND3_RIGHT        (1 << 14)
#define DMG_SND4_RIGHT        (1 << 15)

// n: [0-7]
#define DMG_VOL_LEFT(n)       (((n) & 7) << 0)
#define DMG_VOL_RIGHT(n)      (((n) & 7) << 4)

// n: [0-15]
#define DMG_ENV_VOL(n)        (((n) & 15) << 12)

// n: [0-7]
#define DMG_STEP_TIME(n)      (((n) & 7) << 8)

#define DMG_DIRECTION_DECR    (0 << 11)
#define DMG_DIRECTION_INCR    (1 << 11) 

#define DMG_DUTY_12           (0 << 6)
#define DMG_DUTY_25           (1 << 6)
#define DMG_DUTY_50           (2 << 6)
#define DMG_DUTY_75           (3 << 6)

// n: [0-2]
#define DMG_MASTER_VOL(n)     ((n) % 3)

#define SND_RESET             (1<<15)

#define DMG_FREQ_TIMED        (1<<14)
#define DMG_SWEEP_NUM(n)      ((n) & 7)
#define DMG_SWEEP_UP          (0 << 3)
#define DMG_SWEEP_DOWN        (1 << 3)
#define DMG_SWEEP_STEPTIME(n) (((n) & 7) << 4)

enum {
  REST      = 0,
  NOTE_C2   =44,
  NOTE_CS2  =157,
  NOTE_D2	=263,
  NOTE_DS2  =363,
  NOTE_E2   =457,
  NOTE_F2   =547,
  NOTE_FS2  =631,
  NOTE_G2   =711,
  NOTE_GS2  =786,
  NOTE_A2   =856,
  NOTE_AS2  =923,
  NOTE_B2   =986,
  NOTE_C3   =1046,
  NOTE_CS3  =1102,
  NOTE_D3   =1155,
  NOTE_DS3  =1205,
  NOTE_E3   =1253,
  NOTE_F3   =1297,
  NOTE_FS3  =1339,
  NOTE_G3   =1379,
  NOTE_GS3  =1417,
  NOTE_A3   =1452,
  NOTE_AS3  =1486,
  NOTE_B3   =1517,
  NOTE_C4   =1547,
  NOTE_CS4  =1575,
  NOTE_D4   =1602,
  NOTE_DS4  =1627,
  NOTE_E4   =1650,
  NOTE_F4   =1673,
  NOTE_FS4  =1694,
  NOTE_G4   =1714,
  NOTE_GS4  =1732,
  NOTE_A4   =1750,
  NOTE_AS4  =1767,
  NOTE_B4   =1783,
  NOTE_C5   =1798,
  NOTE_CS5  =1812,
  NOTE_D5   =1825,
  NOTE_DS5  =1837,
  NOTE_E5   =1849,
  NOTE_F5   =1860,
  NOTE_FS5  =1871,
  NOTE_G5   =1881,
  NOTE_GS5  =1890,
  NOTE_A5   =1899,
  NOTE_AS5  =1907,
  NOTE_B5   =1915,
  NOTE_C6   =1923,
  NOTE_CS6  =1930,
  NOTE_D6   =1936,
  NOTE_DS6  =1943,
  NOTE_E6   =1949,
  NOTE_F6   =1954,
  NOTE_FS6  =1959,
  NOTE_G6   =1964,
  NOTE_GS6  =1969,
  NOTE_A6   =1974,
  NOTE_AS6  =1978,
  NOTE_B6   =1982,
  NOTE_C7   =1985,
  NOTE_CS7  =1989,
  NOTE_D7   =1992,
  NOTE_DS7  =1995,
  NOTE_E7   =1998,
  NOTE_F7   =2001,
  NOTE_FS7  =2004,
  NOTE_G7   =2006,
  NOTE_GS7  =2009,
  NOTE_A7   =2011,
  NOTE_AS7  =2013,
  NOTE_B7   =2015,
  NOTE_C8   =2017
} NOTES;

// ============================== MISCELLANEOUS ===============================

// Miscellaneous Functions
int collision(int colA, int rowA, int widthA, int heightA, int colB, int rowB, int widthB, int heightB);


#endif