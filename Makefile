ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM)
endif

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

PRODUCT_NAME = DownwellGBA

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
LIB_DIR := lib
ASS_DIR := assets
GFX_DIR := gfx
SRC_GEN_DIR := srcgen

FPSQRT := $(LIB_DIR)/fpsqrt/fpsqrt.o

LIB := $(FPSQRT)
# EXE := $(BIN_DIR)/pico2gba
# CLI := $(BIN_DIR)/pico2gbacli
# CORE := $(BIN_DIR)/libpico2gba.a
CSRC := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/**/*.c)
CPPSRC := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp)
COBJ := $(CSRC:src/%.c=obj/%.o)
CPPOBJ := $(CPPSRC:src/%.cpp=obj/%.o)

ASSETS := $(call rwildcard, $(ASS_DIR), *.import)
GFX := $(ASSETS:assets/%.import=gfx/%.gfx)

ASSETS_CPP := $(SRC_GEN_DIR)/assets.cpp
ASSETS_O := $(OBJ_DIR)/assets.o

OBJ := $(COBJ) $(CPPOBJ) $(ASSETS_O)


# $(info $(OBJ))

ELF_NAME           = $(BIN_DIR)/$(PRODUCT_NAME).elf
ROM_NAME           = $(BIN_DIR)/$(PRODUCT_NAME).gba
# BIN_NAME           = $(PRODUCT_NAME)

#MODEL              = -mthumb-interwork -mthumb
MODEL              = -mthumb-interwork -marm -mlong-calls #This makes interrupts work
SPECS              = -specs=gba.specs

# --- Archiver
AS                 = $(DEVKITARM)/bin/arm-none-eabi-as
ASFLAGS            = -mthumb-interwork

COMMONCFLAGS   	   = -Werror=return-type

# --- Compiler
CC                 = $(DEVKITARM)/bin/arm-none-eabi-gcc
CFLAGS             = $(MODEL) -O3 -Wall -pedantic -Wextra -std=c99 -D_ROM=$(ROM_NAME) -I$(DEVKITPRO)/libgba/include -Ilib/fpsqrt

# --- C++ Compiler
CPP                = $(DEVKITARM)/bin/arm-none-eabi-g++
CPPFLAGS           = $(MODEL) -O3 -Wall -pedantic -Wextra -D_ROM=$(ROM_NAME) -I$(DEVKITPRO)/libgba/include -Iinclude -Ilib/fpsqrt

# --- Linker
LD                 = $(DEVKITARM)/bin/arm-none-eabi-g++
LDFLAGS            = $(SPECS) $(MODEL) -lm -lstdc++ -L$(DEVKITPRO)/libgba/lib -lgba
# --- Object/Executable Packager
OBJCOPY            = $(DEVKITARM)/bin/arm-none-eabi-objcopy
OBJCOPYFLAGS       = -O binary

# --- ROM Fixer
GBAFIX             = gbafix

IMPORTER           = python3 assetpipeline/assetimport.py

.PHONY: all clean

all: $(ROM_NAME)

# --- Build .elf file into .gba ROM file
$(ROM_NAME) : $(ELF_NAME)
	$(OBJCOPY) $(OBJCOPYFLAGS) $(ELF_NAME) $(ROM_NAME)
	$(GBAFIX) $(ROM_NAME)

# --- Build .o files into .elf file
$(ELF_NAME) : $(OBJ) $(LIB) | $(BIN_DIR)
	$(LD) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
		@mkdir -p $(dir $@)
		$(CC) $(COMMONCFLAGS) $(CFLAGS) -c $< -o $@

$(ASSETS_O) : $(ASSETS_CPP) | $(OBJ_DIR)
		@mkdir -p $(dir $@)
		$(CPP) $(COMMONCFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
		@mkdir -p $(dir $@)
		$(CPP) $(COMMONCFLAGS) $(CPPFLAGS) -c $< -o $@

gfx/%.gfx : $(ASS_DIR)/%.import
		@mkdir -p $(dir $@)
		$(IMPORTER) import $< $@

$(ASSETS_CPP) : $(GFX)
		@mkdir -p $(dir $@)
		$(IMPORTER) build $^ -o $@

$(FPSQRT) : $(LIB_DIR)/fpsqrt/fpsqrt.c
		$(CC) $(COMMONCFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR) $(GFX_DIR):
		mkdir -p $@
		
clean:
		@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)


-include $(OBJ:.o=.d)