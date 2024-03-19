// IGNORE THIS FILE! we give you print.c and print.h for debugging purposes :)

/*
 mgba.h
 Copyright (c) 2016 Jeffrey Pfau
 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:
  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.
 THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define REG_DEBUG_ENABLE (volatile uint16_t*) 0x4FFF780
#define REG_DEBUG_FLAGS (volatile uint16_t*) 0x4FFF700
#define REG_DEBUG_STRING (char*) 0x4FFF600


// Prints a string to mGBA's log with specified LEVEL ( Tools > View Logs )
// Levels are found in print.h
// example usage: mgba_printf_level(MGBA_LOG_DEBUG, "String");
// useful if you want to categorize your print statements / error codes
void mgba_printf_level(int level, const char* ptr, ...) {
#if !MODERN
	va_list args;
	level &= 0x7;
	va_start(args, ptr);
	vsnprintf(REG_DEBUG_STRING, 0x100, ptr, args);
	va_end(args);
	*REG_DEBUG_FLAGS = level | 0x100;
#endif
}

// Prints a string to mGBA's log under WARNING by default ( Tools > View Logs )
// Warning should be checked by default in mGBA
void mgba_printf(const char* ptr, ...) {
#if !MODERN
	// defaulting to warning text
	int level = 2;
	va_list args;
	level &= 0x7;
	va_start(args, ptr);
	vsnprintf(REG_DEBUG_STRING, 0x100, ptr, args);
	va_end(args);
	*REG_DEBUG_FLAGS = level | 0x100;
#endif
}

// Breaks program -- useful for critical breakpoints
// Pops up with a breakpoint window when reached
void mgba_break(void) {
	mgba_printf_level(0, "Breakpoint Reached");
}

// Initialize debugging -- place in your initialize fuction
uint8_t mgba_open(void) {
	*REG_DEBUG_ENABLE = 0xC0DE;
	return *REG_DEBUG_ENABLE == 0x1DEA;
}

// Deinitialize debugging
void mgba_close(void) {
	*REG_DEBUG_ENABLE = 0;
}