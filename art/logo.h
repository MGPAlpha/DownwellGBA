/*
 * Exported with nin10kit v1.8
 * Invocation command was nin10kit --force --bpp=4 --mode=sprites --transparent=ff00ff logo logo.png 
 * Time-stamp: Monday 04/04/2022, 14:11:49
 * 
 * Image Information
 * -----------------
 * logo.png 128@256
 * Transparent color: (255, 0, 255)
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef LOGO_H
#define LOGO_H

#define LOGO_PALETTE_TYPE (0 << 13)
#define LOGO_DIMENSION_TYPE (1 << 6)

extern const unsigned short logo_palette[16];
#define LOGO_PALETTE_SIZE 32
#define LOGO_PALETTE_LENGTH 16

extern const unsigned short logo[8192];
#define LOGO_SIZE 16384
#define LOGO_LENGTH 8192

#define LOGO_PALETTE_ID (0 << 12)
#define LOGO_ID 0

#endif

