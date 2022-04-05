/*
 * Exported with nin10kit v1.8
 * Invocation command was nin10kit --force --mode=sprites --bpp=4 --transparent=ff00ff dither dither.png 
 * Time-stamp: Monday 04/04/2022, 19:42:26
 * 
 * Image Information
 * -----------------
 * dither.png 8@56
 * Transparent color: (255, 0, 255)
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef DITHER_H
#define DITHER_H

#define DITHER_PALETTE_TYPE (0 << 13)
#define DITHER_DIMENSION_TYPE (1 << 6)

extern const unsigned short dither_palette[16];
#define DITHER_PALETTE_SIZE 32
#define DITHER_PALETTE_LENGTH 16

extern const unsigned short dither[112];
#define DITHER_SIZE 224
#define DITHER_LENGTH 112

#define DITHER_PALETTE_ID (0 << 12)
#define DITHER_ID 0

#endif

