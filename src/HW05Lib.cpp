#include "HW05Lib.hpp"

extern "C" {

#include "print.h"

}

// The start of the video memory
unsigned short *videoBuffer = (unsigned short *)0x6000000;

// The start of DMA registers
DMA *dma = (DMA *)0x40000B0;

// Set a pixel on the screen in Mode 3
void setPixel3(Vector2 pos, unsigned short color) {
	videoBuffer[OFFSET(int(pos.x), int(pos.y), SCREENWIDTH)] = color;
}

// Set a pixel on the screen in Mode 4
void setPixel4(Vector2 pos, unsigned char colorIndex) {
    volatile unsigned short pixelData = videoBuffer[OFFSET(int(pos.x), int(pos.y), SCREENWIDTH) / 2];
    if (int(pos.x) & 1)
    {
        pixelData &= 0x00FF;
        pixelData |= colorIndex << 8;
    }
    else
    {
        pixelData &= 0xFF00;
        pixelData |= colorIndex;
    }
    videoBuffer[OFFSET(int(pos.x), int(pos.y), SCREENWIDTH) / 2] = pixelData;}

// Draw a rectangle at the specified location and size in Mode 3
void drawRect3(Rect rect, volatile unsigned short color) {
	for(int r = 0; r < int(rect.size.y); r++) {
        DMANow(3, &color, &videoBuffer[OFFSET(int(rect.pos.x), int(rect.pos.y) + r, SCREENWIDTH)], DMA_SOURCE_FIXED | int(rect.size.x));
	}
}

// Draw a rectangle at the specified location and size in Mode 4
void drawRect4(Rect rect, volatile unsigned char colorIndex) {
    volatile unsigned short pixelData = colorIndex | (colorIndex << 8);
    for (int r = 0; r < int(rect.size.y); r++)
    {
        // Ensure we don't DMA 0 copies
        if (rect.size.x == 1)
            setPixel4(rect.pos + Vector2(0,r), colorIndex);
        else if (rect.size.x == 2)
        {
            setPixel4(rect.pos + Vector2(0,r), colorIndex);
            setPixel4(rect.pos + Vector2(1,r), colorIndex);
        }
        else if ((int(rect.pos.x) & 1) && (int(rect.pos.y) & 1)) // Odd width odd col
        {
            setPixel4(rect.pos + Vector2(0,r), colorIndex);
            DMANow(3, &pixelData, &videoBuffer[OFFSET(int(rect.pos.x) + 1, int(rect.pos.y) + r, SCREENWIDTH) / 2], DMA_SOURCE_FIXED | (int(rect.size.x) - 1) / 2);
        }
        else if (int(rect.size.x) & 1) // Even col odd width
        {
            DMANow(3, &pixelData, &videoBuffer[OFFSET(int(rect.pos.x), int(rect.pos.y) + r, SCREENWIDTH) / 2], DMA_SOURCE_FIXED | (int(rect.size.x) - 1) / 2);
            setPixel4((Vector2){rect.pos.x + rect.size.x - 1, rect.pos.y + r}, colorIndex);
        }
        else if (int(rect.pos.x) & 1) // Odd col even width
        {
            setPixel4(rect.pos + Vector2(0,r), colorIndex);
            DMANow(3, &pixelData, &videoBuffer[OFFSET(int(rect.pos.x) + 1, int(rect.pos.y) + r, SCREENWIDTH) / 2], DMA_SOURCE_FIXED | (int(rect.size.x) - 2) / 2);
            setPixel4((Vector2){rect.pos.x + rect.size.x - 1, rect.pos.y + r}, colorIndex);
        }
        else // Even col even width
        {
            DMANow(3, &pixelData, &videoBuffer[OFFSET(int(rect.pos.x), int(rect.pos.y) + r, SCREENWIDTH) / 2], DMA_SOURCE_FIXED | int(rect.size.x) / 2);
        }
    }}

// Fill the entire screen with a single color in Mode 3
void fillScreen3(volatile unsigned short color) {
    unsigned short c = color;
	DMANow(3, &c, videoBuffer, DMA_SOURCE_FIXED | (SCREENWIDTH * SCREENHEIGHT));
}

// Fill the entire screen with a single color in Mode 4
void fillScreen4(volatile unsigned char colorIndex) {
    // Double up our colorIndex to set 2 pixels per transfer (DMA by default transfers 2 bytes at a time)
    volatile unsigned short pixelData = colorIndex | (colorIndex << 8);
    DMANow(3, &pixelData, videoBuffer, DMA_SOURCE_FIXED | (SCREENWIDTH * SCREENHEIGHT) / 2);
}

// Draw an image at the specified location and size in Mode 3
void drawImage3(Vector2 pos, Vector2 size, const unsigned short *image) {
    for(int r = 0; r < size.y; r++) {
        DMANow(3, &image[OFFSET(0, r, int(size.x))], &videoBuffer[OFFSET(int(pos.x), int(pos.y) + r, SCREENWIDTH)], int(size.x));
    }
}

// Draw an image at the specified location and size in Mode 4 (must be even col and width)
void drawImage4(Vector2 pos, Vector2 size, const unsigned short *image) {
    for (int i = 0; i < size.y; i++)
    {
        if (pos.y + i < 0) continue;
        DMANow(3, &image[OFFSET(0, i, int(size.x) / 2)], &videoBuffer[OFFSET(int(pos.x), int(pos.y) + i, SCREENWIDTH) / 2], int(size.x) / 2);
    }
}

// Fill the entire screen with an image in Mode 3
void drawFullscreenImage3(const unsigned short *image) {
    DMANow(3, image, videoBuffer, SCREENWIDTH * SCREENHEIGHT);
}

// Fill the entire screen with an image in Mode 4
void drawFullscreenImage4(const unsigned short *image) {
    DMANow(3, image, videoBuffer, SCREENWIDTH * SCREENHEIGHT / 2);
}

// Pause code execution until vertical blank begins
void waitForVBlank() {
	while(SCANLINECOUNTER >= 160);
	while(SCANLINECOUNTER < 160);
}

void waitNVBlanks(int n) {
    for (int i = 0; i < n; i++) {
        waitForVBlank();
    }
}

// Flips the page
void flipPage() {
    if (REG_DISPCTL & DISP_BACKBUFFER) {
        videoBuffer = BACKBUFFER;
    } else {
        videoBuffer = FRONTBUFFER;
    }
    REG_DISPCTL ^= DISP_BACKBUFFER;
}

unsigned short oldButtons;
unsigned short buttons;

// Set up and begin a DMA transfer
void DMANow(int channel, volatile const void *src, volatile void *dst, unsigned int cnt) {
    // Turn DMA off
    dma[channel].cnt = 0;

    // Set source and destination
    dma[channel].src = src;
    dma[channel].dst = dst;

    // Set control and begin
    dma[channel].cnt = cnt | DMA_ON;}
