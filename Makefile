TARGET = app
FONTDIR = libs/McuFont/fonts
MFDIR = libs/McuFont/decoder
include $(MFDIR)/mcufont.mk

OS = $(shell uname -s)

# Flags and compiler
CC = gcc
CFLAGS = -Iinc -I$(FONTDIR) -I$(MFDIR) -Wall -Wextra
DEBUG_CFLAGS = "-g"
LDFLAGS = -lSDL2 -lm 

ifeq ($(OS), Darwin) 
	CFLAGS += -I/opt/homebrew/Cellar/sdl2/2.30.10/include/SDL2
	LDFLAGS += -L/opt/homebrew/Cellar/sdl2/2.30.10/lib
endif

# Directory
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = build

# Sources and objects
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

# Default build
all: $(TARGET)

# Build with debug flags
debug: CFLAGS += $(DEBUG_CFLAGS)
debug: $(TARGET)

# Executable build
$(TARGET): $(OBJECTS)
	@echo "Building for target: $(OS)"
	$(CC) $(OBJECTS) -o $@ $(MFSRC) $(CFLAGS) $(LDFLAGS)

# Object file compilation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

# Build dir creation
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	@echo "Cleaning up"
	rm -rf $(OBJ_DIR) $(TARGET) app.dSYM
	
test: $(TARGET)
	./$<

.PHONY: all clean

