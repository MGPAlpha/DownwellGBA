ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM)
endif

PRODUCT_NAME = DownwellGBA

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
LIB_DIR := lib

LIB := 
# EXE := $(BIN_DIR)/pico2gba
# CLI := $(BIN_DIR)/pico2gbacli
# CORE := $(BIN_DIR)/libpico2gba.a
CSRC := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/**/*.c)
CPPSRC := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp)
COBJ := $(CSRC:src/%.c=obj/%.o)
CPPOBJ := $(CPPSRC:src/%.cpp=obj/%.o)

OBJ := $(COBJ) $(CPPOBJ)


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
CFLAGS             = $(MODEL) -O3 -Wall -pedantic -Wextra -std=c99 -D_ROM=$(ROM_NAME) -I$(DEVKITPRO)/libgba/include

# --- C++ Compiler
CPP                = $(DEVKITARM)/bin/arm-none-eabi-g++
CPPFLAGS           = $(MODEL) -O3 -Wall -pedantic -Wextra -D_ROM=$(ROM_NAME) -I$(DEVKITPRO)/libgba/include

# --- Linker
LD                 = $(DEVKITARM)/bin/arm-none-eabi-g++
LDFLAGS            = $(SPECS) $(MODEL) -lm -lstdc++ -L$(DEVKITPRO)/libgba/lib -lgba
# --- Object/Executable Packager
OBJCOPY            = $(DEVKITARM)/bin/arm-none-eabi-objcopy
OBJCOPYFLAGS       = -O binary

# --- ROM Fixer
GBAFIX             = gbafix

.PHONY: all clean

all: $(ROM_NAME)

# --- Build .elf file into .gba ROM file
$(ROM_NAME) : $(ELF_NAME)
	$(OBJCOPY) $(OBJCOPYFLAGS) $(ELF_NAME) $(ROM_NAME)
	$(GBAFIX) $(ROM_NAME)

# --- Build .o files into .elf file
$(ELF_NAME) : $(OBJ) | $(BIN_DIR)
	$(LD) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
		@mkdir -p $(dir $@)
		$(CC) $(COMMONCFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
		@mkdir -p $(dir $@)
		$(CPP) $(COMMONCFLAGS) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
		mkdir -p $@
		
clean:
		@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)


-include $(OBJ:.o=.d)