CC = gcc
CFLAGS = -Wall -g -Iinclude
LDFLAGS = -lm

EXECUTABLE = main
SRC_DIR = src
APP_DIR = app
OBJ_DIR = obj

SOURCES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(APP_DIR)/*.c)

OBJECTS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SOURCES)))

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(APP_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Limpando arquivos gerados..."
	rm -rf $(OBJ_DIR) $(EXECUTABLE)

.PHONY: all clean