/*
 * Exported with nin10kit v1.8
 * Invocation command was nin10kit --mode=0 --bpp=4 --transparent=ff00ff title title-bg.png sky.png 
 * Time-stamp: Monday 04/04/2022, 19:29:44
 * 
 * Image Information
 * -----------------
 * title-bg.png 512@512
 * sky.png 256@256
 * Transparent color: (255, 0, 255)
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef TITLE_H
#define TITLE_H

extern const unsigned short title_palette[16];
#define TITLE_PALETTE_SIZE 32
#define TITLE_PALETTE_LENGTH 16

#define TITLE_PALETTE_TYPE (0 << 7)

extern const unsigned short title_tiles[5728];
#define TITLE_TILES 358
#define TITLE_TILES_SIZE 11456
#define TITLE_TILES_LENGTH 5728

extern const unsigned short titlebg[4096];
#define TITLEBG_MAP_WIDTH 64
#define TITLEBG_MAP_HEIGHT 64
#define TITLEBG_MAP_SIZE 8192
#define TITLEBG_MAP_LENGTH 4096
#define TITLEBG_MAP_TYPE (3 << 14)

extern const unsigned short sky[1024];
#define SKY_MAP_WIDTH 32
#define SKY_MAP_HEIGHT 32
#define SKY_MAP_SIZE 2048
#define SKY_MAP_LENGTH 1024
#define SKY_MAP_TYPE (0 << 14)

#endif

