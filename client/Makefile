CLIENT_NAME := sh13.exe

TERMINAL := terminator
TERMINAL_FLAGS := --new-tab
TERMINAL_EXEC_FLAG := -e

MODE := DEBUG

CC := gcc
CFLAGS := -I/usr/include/SDL2 -Wall
DEBUG_FLAGS := -O0 -g
CLIBS := -lSDL2_image -lSDL2_ttf -lSDL2 -lpthread

ifeq ($(MODE), DEBUG)
	CFLAGS += $(DEBUG_FLAGS)
endif

all: build_client

client: sh13.o
	@echo "Linking $^ & libraries.."
	@$(CC) $(CFLAGS) -o $(CLIENT_NAME) $^ $(CLIBS)
	@echo "Build of the client done!"
	@echo ""

%.o : %.c
	@echo "Compiling $<.."
	@$(CC) $(CFLAGS) -c $< $(CLIBS)

clean:
	rm -rf *.o

msproper: clean
	rm -rf $(CLIENT_NAME)
