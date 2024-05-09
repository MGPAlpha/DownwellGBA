/*
 * Exported with nin10kit v1.8
 * Invocation command was nin10kit --force --bpp=4 --mode=sprites --transparent=ff00ff spritesheet spritesheet.png 
 * Time-stamp: Friday 04/29/2022, 03:35:03
 * 
 * Image Information
 * -----------------
 * spritesheet.png 256@256
 * Transparent color: (255, 0, 255)
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#define SPRITESHEET_PALETTE_TYPE (0 << 13)
#define SPRITESHEET_DIMENSION_TYPE (1 << 6)

extern const unsigned short spritesheet_palette[16];
#define SPRITESHEET_PALETTE_SIZE 32
#define SPRITESHEET_PALETTE_LENGTH 16

extern const unsigned short spritesheet[16384];
#define SPRITESHEET_SIZE 32768
#define SPRITESHEET_LENGTH 16384

#define SPRITESHEET_PALETTE_ID (0 << 12)
#define SPRITESHEET_ID 0

#endif

