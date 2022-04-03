/*
 * Exported with nin10kit v1.8
 * Invocation command was nin10kit --mode=0 --bpp=4 --transparent=ff00ff instructions instructions.png 
 * Time-stamp: Saturday 04/02/2022, 23:14:18
 * 
 * Image Information
 * -----------------
 * instructions.png 256@256
 * Transparent color: (255, 0, 255)
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

extern const unsigned short instructions_palette[16];
#define INSTRUCTIONS_PALETTE_SIZE 32
#define INSTRUCTIONS_PALETTE_LENGTH 16

#define INSTRUCTIONS_PALETTE_TYPE (0 << 7)

extern const unsigned short instructions_tiles[1248];
#define INSTRUCTIONS_TILES 78
#define INSTRUCTIONS_TILES_SIZE 2496
#define INSTRUCTIONS_TILES_LENGTH 1248

extern const unsigned short instructions[1024];
#define INSTRUCTIONS_MAP_WIDTH 32
#define INSTRUCTIONS_MAP_HEIGHT 32
#define INSTRUCTIONS_MAP_SIZE 2048
#define INSTRUCTIONS_MAP_LENGTH 1024
#define INSTRUCTIONS_MAP_TYPE (0 << 14)

#endif

