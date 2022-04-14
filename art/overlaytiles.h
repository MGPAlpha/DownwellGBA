/*
 * Exported with nin10kit v1.8
 * Invocation command was nin10kit --force --mode=sprites --bpp=4 --transparent=ff00ff overlaytiles overlaytiles.png 
 * Time-stamp: Wednesday 04/13/2022, 22:11:51
 * 
 * Image Information
 * -----------------
 * overlaytiles.png 256@128
 * Transparent color: (255, 0, 255)
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef OVERLAYTILES_H
#define OVERLAYTILES_H

#define OVERLAYTILES_PALETTE_TYPE (0 << 13)
#define OVERLAYTILES_DIMENSION_TYPE (1 << 6)

extern const unsigned short overlaytiles_palette[16];
#define OVERLAYTILES_PALETTE_SIZE 32
#define OVERLAYTILES_PALETTE_LENGTH 16

extern const unsigned short overlaytiles[8192];
#define OVERLAYTILES_SIZE 16384
#define OVERLAYTILES_LENGTH 8192

#define OVERLAYTILES_PALETTE_ID (0 << 12)
#define OVERLAYTILES_ID 0

#endif

