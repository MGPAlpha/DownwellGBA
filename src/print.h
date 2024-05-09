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
#ifndef MGBA_H
#define MGBA_H

#include <stdint.h>

// Debug Levels: use in mgba_printf_level
#define MGBA_LOG_FATAL 0  // Fatal category   - will stop the game with a fatal error
#define MGBA_LOG_ERROR 1  // Error category   - use for critical messages
#define MGBA_LOG_WARN  2  // Warning category - use for warning messages
#define MGBA_LOG_INFO  3  // Info category    - use for informational messages
#define MGBA_LOG_DEBUG 4  // Debug category   - use for debugging messages

// Function Prototypes
void mgba_printf_level(int level, const char* ptr, ...);
void mgba_printf(const char* string, ...);
void mgba_break(void);
uint8_t mgba_open(void);
void mgba_close(void);

#endif
