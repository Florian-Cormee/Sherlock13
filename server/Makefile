SERVER_NAME := ../server.exe

CC := gcc
CFLAGS := -Wall
DEBUG_FLAGS := -O0 -g
CLIBS :=

ifeq ($(MODE), DEBUG)
	CFLAGS += $(DEBUG_FLAGS)
endif

server: cartes.o com.o server.o msg.o
	@echo "Linking $^ & libraries.."
	@$(CC) $(CFLAGS) -o $(SERVER_NAME) $^ $(CLIBS)
	@echo "Build of the server done!"
	@echo ""

%.o : %.c
	@echo "Compiling $<.."
	@$(CC) $(CFLAGS) -c $< $(CLIBS)

clean:
	rm -rf *.o

msproper: clean
	rm -rf $(CLIENT_NAME) $(SERVER_NAME)
