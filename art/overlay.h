/*
 * Exported with nin10kit v1.8
 * Invocation command was nin10kit --mode=0 --bpp=4 --transparent=ff00ff overlay overlay.png 
 * Time-stamp: Saturday 04/02/2022, 22:53:57
 * 
 * Image Information
 * -----------------
 * overlay.png 256@256
 * Transparent color: (255, 0, 255)
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef OVERLAY_H
#define OVERLAY_H

extern const unsigned short overlay_palette[16];
#define OVERLAY_PALETTE_SIZE 32
#define OVERLAY_PALETTE_LENGTH 16

#define OVERLAY_PALETTE_TYPE (0 << 7)

extern const unsigned short overlay_tiles[128];
#define OVERLAY_TILES 8
#define OVERLAY_TILES_SIZE 256
#define OVERLAY_TILES_LENGTH 128

extern const unsigned short overlay[1024];
#define OVERLAY_MAP_WIDTH 32
#define OVERLAY_MAP_HEIGHT 32
#define OVERLAY_MAP_SIZE 2048
#define OVERLAY_MAP_LENGTH 1024
#define OVERLAY_MAP_TYPE (0 << 14)

#endif

