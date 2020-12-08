BIN = socket_server
DATE = $(shell date +%Y%m%d)
BIN_NAME = $(BIN)_$(DATE)

CC = gcc
BUILD_DIR = ./.build
SRC_PATH = src
INCLUDE_PATH = inc

SRC_FILE = $(wildcard $(SRC_PATH)/*.c)
INCLUDE_FILE = $(wildcard $(INCLUDE_PATH)/*.h)
OBJ_FILE = $(patsubst $(SRC_PATH)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILE))
CFLAG = -Wall $(patsubst %,-I%,$(INCLUDE_PATH))

all: $(BIN_NAME)

$(BIN_NAME): $(OBJ_FILE)
	$(CC) $^ -o $@

$(OBJ_FILE): $(BUILD_DIR)/%.o: $(SRC_PATH)/%.c $(INCLUDE_FILE)
	$(shell mkdir -p $(BUILD_DIR))
	$(CC) -o $@ $(CFLAG) -c $<

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(BIN)*

.PHONY: clean all