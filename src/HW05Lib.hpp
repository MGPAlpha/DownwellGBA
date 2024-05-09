/*
 * Based on a modified version of HW04Lib
 */

#ifndef HW05_LIB_HPP
#define HW05_LIB_HPP

#include "engine/gbamath.hpp"

// Typedefs & Structs
typedef unsigned short u16;
typedef u16 color;

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
void setPixel3(GBAEngine::Vector2 pos, unsigned short color);
void drawRect3(GBAEngine::Rect rect, volatile unsigned short color);
void fillScreen3(volatile unsigned short color);
void drawImage3(GBAEngine::Vector2 pos, GBAEngine::Vector2 size, const unsigned short *image);
void drawFullscreenImage3(const unsigned short *image);

// Mode 4 Drawing Functions
void setPixel4(GBAEngine::Vector2 pos, unsigned char colorIndex);
void drawRect4(GBAEngine::Rect rect, volatile unsigned char colorIndex);
void fillScreen4(volatile unsigned char colorIndex);
void drawImage4(GBAEngine::Vector2 pos, GBAEngine::Vector2 size, const unsigned short *image);
void drawFullscreenImage4(const unsigned short *image);

// Miscellaneous Drawing Functions
void waitForVBlank();
void waitNVBlanks(int n);
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

// ================================ SAVE DATA =================================

#define GAMEPAK_RAM  ((volatile unsigned char*)0x0E000000)

// =================================== DMA ====================================

// DMA Struct
struct DMA {
    volatile const void *src;
    volatile void *dst;
    volatile unsigned int cnt;
};

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

// =================================== TIMERS ====================================

// Controllers
#define REG_TM0CNT *(volatile unsigned short*)0x4000102
#define REG_TM1CNT *(volatile unsigned short*)0x4000106
#define REG_TM2CNT *(volatile unsigned short*)0x400010A
#define REG_TM3CNT *(volatile unsigned short*)0x400010E

// Timer values
#define REG_TM0D       *(volatile unsigned short*)0x4000100
#define REG_TM1D       *(volatile unsigned short*)0x4000104
#define REG_TM2D       *(volatile unsigned short*)0x4000108
#define REG_TM3D       *(volatile unsigned short*)0x400010C

// Timer flags
#define TIMER_ON      (1<<7)
#define TIMER_OFF     (0<<7)
#define TM_IRQ        (1<<6)
#define TM_CASCADE    (1<<2)
#define TM_FREQ_1     0
#define TM_FREQ_64    1
#define TM_FREQ_256   2
#define TM_FREQ_1024  3

// =================================== INTERRUPTS ====================================

// Controller
#define REG_IME *(unsigned short*)0x4000208
// Enabler
#define REG_IE *(unsigned short*)0x4000200
// Flag
#define REG_IF *(volatile unsigned short*)0x4000202

// Interrupt handler function pointer
typedef void (*ihp)(void);
// Interrupt handler register
#define REG_INTERRUPT *((ihp*)0x03007FFC)
// Display status register
#define REG_DISPSTAT *(unsigned short*)0x4000004

// Interrupt constants for turning them on
#define INT_VBLANK_ENABLE 1 << 3

// Interrupt constants for checking which type of interrupt happened 
#define INT_VBLANK 1 << 0   
#define INT_TM0 1<<3    
#define INT_TM1 1<<4    
#define INT_TM2 1<<5    
#define INT_TM3 1<<6    
#define INT_BUTTON 1 << 12

// ============================== MISCELLANEOUS ===============================

// Miscellaneous Functions
int collision(int colA, int rowA, int widthA, int heightA, int colB, int rowB, int widthB, int heightB);


#endif