TARGET    := sgl_simulator
BUILD_DIR := build

ENV_TYPE := $(shell echo $$0)
ifeq ($(ENV_TYPE),/usr/bin/sh)
    CONFIG = cp sgl_config.h ../sgl/source/sgl_config.h
    SDL_COPY = cp  sdl/bin/SDL2.dll $(BUILD_DIR)/SDL2.dll
    CLEAN = rm -rf $(BUILD_DIR)
else
    CONFIG = copy /Y sgl_config.h ..\sgl\source\sgl_config.h
    SDL_COPY = copy /Y sdl\bin\SDL2.dll $(BUILD_DIR)\SDL2.dll
    CLEAN = rmdir /S /Q $(BUILD_DIR)
endif


# toolchain
CC_PREFIX ?= 
CC = $(CC_PREFIX)gcc
AS = $(CC_PREFIX)gcc -x assembler-with-cpp
CP = $(CC_PREFIX)objcopy
SZ = $(CC_PREFIX)size
OD = $(CC_PREFIX)objdump
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

CPATH     := -Isdl/include/SDL2      \
			 -I../source

CFLAGS    := $(CPATH) -O2 -Wl,-Bstatic -ffunction-sections -fdata-sections -nostdlib -ffreestanding -Wunused-function -Wall -Wextra -std=c99  -g
LDFLAGS   := -lmingw32 -lSDL2main -lSDL2.dll -Lsdl/lib -Wl,-Map=$(BUILD_DIR)/$(TARGET).map


SOURCE    := main.c sgl_port_sdl2.c \
			../source/sgl_core.c    \
			../source/sgl_draw.c    \
			../source/sgl_mm.c      \
			../source/sgl_widget.c  \
			../source/sgl_ascii_consolas24.c 


.PHONY: config all
all: config $(BUILD_DIR)/$(TARGET).exe elf_info


# list of c and c++ program objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(patsubst %.c, %.o, $(SOURCE))))
vpath %.c $(sort $(dir $(SOURCE)))


$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo "CC   $<"
	@$(CC) -c $(CFLAGS) -MMD -MP \
		-MF  $(BUILD_DIR)/$(notdir $(<:.c=.d)) \
		-Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@


$(BUILD_DIR)/$(TARGET).exe: $(OBJECTS) Makefile
	@echo "LD   $@"
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@$(OD) $(BUILD_DIR)/$(TARGET).exe -xS > $(BUILD_DIR)/$(TARGET).s $@
	@echo ''
	@echo "Build Successful!"
	@echo "ELF   $@"


elf_info: $(BUILD_DIR)/$(TARGET).exe
	@echo "=================================================================="
	@$(SZ) $<
	@echo "=================================================================="


$(BUILD_DIR):
	@mkdir $@


# Pseudo command
.PHONY: clean run


run: $(BUILD_DIR)/$(TARGET).exe
	@$(SDL_COPY)
	@$(BUILD_DIR)/$(TARGET).exe


# clean command, delete build directory
clean:
	@$(CLEAN)
