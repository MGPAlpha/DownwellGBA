/*
 * Exported with nin10kit v1.8
 * Invocation command was nin10kit --force --mode=sprites --bpp=4 --transparent=ff00ff terraintiles terraintiles.png 
 * Time-stamp: Tuesday 04/12/2022, 14:22:37
 * 
 * Image Information
 * -----------------
 * terraintiles.png 256@128
 * Transparent color: (255, 0, 255)
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef TERRAINTILES_H
#define TERRAINTILES_H

#define TERRAINTILES_PALETTE_TYPE (0 << 13)
#define TERRAINTILES_DIMENSION_TYPE (1 << 6)

extern const unsigned short terraintiles_palette[16];
#define TERRAINTILES_PALETTE_SIZE 32
#define TERRAINTILES_PALETTE_LENGTH 16

extern const unsigned short terraintiles[8192];
#define TERRAINTILES_SIZE 16384
#define TERRAINTILES_LENGTH 8192

#define TERRAINTILES_PALETTE_ID (0 << 12)
#define TERRAINTILES_ID 0

#endif

