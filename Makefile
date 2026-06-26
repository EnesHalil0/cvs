CC := gcc
MATLAB := /c/Program\ Files/MATLAB/R2026a/bin/matlab.exe

TARGET := cvs.exe
PYTHON := ./.venv/bin/python.exe
OUTPUT_CSV := output.csv
REFERENCE_CSV := reference.csv

SRC_DIR     := src
BUILD_DIR   := build
INCLUDE_DIR := src/cvs

# CFLAGS  := -Wall -Wextra -O3 -I$(INCLUDE_DIR)
CFLAGS  := -Wall -Wextra -O0 -ggdb -I$(INCLUDE_DIR)
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

run: $(TARGET)
	./$(TARGET)

plot: $(TARGET)
	./$(TARGET)
	$(PYTHON) scripts/plot_cvs_output.py --csv $(OUTPUT_CSV)

compare-plot: $(TARGET)
	./$(TARGET)
	$(MATLAB) -batch "addpath('scripts'); export_reference_cvs('$(REFERENCE_CSV)', 40);"
	$(MATLAB) -r "addpath('scripts'); plot_cvs_comparison('$(OUTPUT_CSV)', '$(REFERENCE_CSV)');"

reference:
	$(MATLAB) -batch "addpath('scripts'); export_reference_cvs('$(REFERENCE_CSV)', 40);"

.PHONY: all clean rebuild run plot compare-plot reference
