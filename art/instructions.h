/*
 * Exported with nin10kit v1.8
 * Invocation command was nin10kit --mode=0 --bpp=4 --transparent=ff00ff instructions instructions.png 
 * Time-stamp: Friday 04/29/2022, 00:23:05
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

extern const unsigned short instructions_tiles[1392];
#define INSTRUCTIONS_TILES 87
#define INSTRUCTIONS_TILES_SIZE 2784
#define INSTRUCTIONS_TILES_LENGTH 1392

extern const unsigned short instructions[1024];
#define INSTRUCTIONS_MAP_WIDTH 32
#define INSTRUCTIONS_MAP_HEIGHT 32
#define INSTRUCTIONS_MAP_SIZE 2048
#define INSTRUCTIONS_MAP_LENGTH 1024
#define INSTRUCTIONS_MAP_TYPE (0 << 14)

#endif

