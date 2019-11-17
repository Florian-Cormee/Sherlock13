CLIENT_NAME := sh13.exe
SERVER_NAME := server.exe

SERVER_PORT := 32000
CLIENT1_PORT := 32001
CLIENT2_PORT := 32002
CLIENT3_PORT := 32003
CLIENT4_PORT := 32004

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

all: client server

.PHONY: server clean msproper

client: sh13.o
	@echo "Linking $^ & libraries.."
	@$(CC) $(CFLAGS) -o $(CLIENT_NAME) $^ $(CLIBS)
	@echo "Build of the client done!"
	@echo ""

server:
	$(MAKE) -C server/ server

clean:
	$(MAKE) -C server/ $@

msproper:
	$(MAKE) -C server/ $@

run_all: run_server run_client1 run_client2 run_client3 run_client4
run_server: server
	@echo Starting the server on port: $(SERVER_PORT)
	@$(TERMINAL) $(TERMINAL_FLAGS) $(TERMINAL_EXEC_FLAG) "./$(SERVER_NAME) $(SERVER_PORT)"

run_client1: client
	@echo Starting client 1 on port: $(CLIENT1_PORT)
	@$(TERMINAL) $(TERMINAL_FLAGS) $(TERMINAL_EXEC_FLAG) "./$(CLIENT_NAME) localhost $(SERVER_PORT) localhost $(CLIENT1_PORT) Alice"

run_client2: client
	@echo Starting client 2 on port: $(CLIENT2_PORT)
	@$(TERMINAL) $(TERMINAL_FLAGS) $(TERMINAL_EXEC_FLAG) "./$(CLIENT_NAME) localhost $(SERVER_PORT) localhost $(CLIENT2_PORT) Bob"

run_client3: client
	@echo Starting client 3 on port: $(CLIENT3_PORT)
	@$(TERMINAL) $(TERMINAL_FLAGS) $(TERMINAL_EXEC_FLAG) "./$(CLIENT_NAME) localhost $(SERVER_PORT) localhost $(CLIENT3_PORT) Charlie"

run_client4: client
	@echo Starting client 4 on port: $(CLIENT4_PORT)
	@$(TERMINAL) $(TERMINAL_FLAGS) $(TERMINAL_EXEC_FLAG) "./$(CLIENT_NAME) localhost $(SERVER_PORT) localhost $(CLIENT4_PORT) Denis"
