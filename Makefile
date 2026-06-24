CC := gcc

TARGET := cvs.exe

SRC_DIR     := src
BUILD_DIR   := build
INCLUDE_DIR := src/cvs

CFLAGS  := -Wall -Wextra -O3 -I$(INCLUDE_DIR)
LDFLAGS :=

SRC_DIR := src
BUILD_DIR := build

SRCS := $(shell find $(SRC_DIR) -type f -name "*.c")
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))



all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all

.PHONY: all clean rebuild