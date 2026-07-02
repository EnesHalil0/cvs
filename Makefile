CC := gcc
MATLAB := /c/Program\ Files/MATLAB/R2026a/bin/matlab.exe

TARGET := cvs.exe
PYTHON := ./.venv/bin/python.exe
OUTPUT_CSV := output.csv
REFERENCE_CSV := reference.csv
WEB_BUILD_DIR = $(BUILD_DIR)/web
WEB_TEMPLATE := web/index.html

SRC_DIR     := src
BUILD_DIR   := build
INCLUDE_DIR := src/cvs
WEB_INCLUDE_DIR := src/web

# CFLAGS  := -Wall -Wextra -O3 -I$(INCLUDE_DIR)
CFLAGS  := -Wall -Wextra -O0 -ggdb -I$(INCLUDE_DIR)
LDFLAGS :=

SRC_DIR := src
BUILD_DIR := build

SRCS := $(shell find $(SRC_DIR)/cvs -type f -name "*.c") $(SRC_DIR)/main.c
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

LOCAL_EMCC := $(firstword $(wildcard .tools/emsdk/upstream/emscripten/emcc.exe) $(wildcard .tools/emsdk/upstream/emscripten/emcc.bat) $(wildcard .tools/emsdk/upstream/emscripten/emcc))
SYSTEM_EMCC := $(shell command -v emcc 2>/dev/null)
EMCC ?= $(if $(LOCAL_EMCC),$(LOCAL_EMCC),$(SYSTEM_EMCC))
WEB_SRCS := $(shell find $(SRC_DIR)/cvs -type f -name "*.c") $(SRC_DIR)/web/cvs_wasm_api.c
WEB_CFLAGS := -I$(INCLUDE_DIR) -I$(WEB_INCLUDE_DIR) -O3
WEB_LDFLAGS := -s WASM=1 \
	-s MODULARIZE=1 \
	-s EXPORT_NAME=createCVSModule \
	-s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
	-s EXPORTED_FUNCTIONS='["_cvs_wasm_init","_cvs_wasm_init_ex","_cvs_wasm_step","_cvs_wasm_run_steps","_cvs_wasm_get_time","_cvs_wasm_get_volume","_cvs_wasm_get_pressure","_cvs_wasm_get_flow","_cvs_wasm_get_e_lv","_cvs_wasm_get_e_rv"]'



all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/cvs $(BUILD_DIR)/main.o $(TARGET)

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

web:
	powershell.exe -NoProfile -ExecutionPolicy Bypass -File scripts/build_wasm.ps1

web-clean:
	rm -rf $(WEB_BUILD_DIR)

help:
	@echo "make              Build native cvs.exe"
	@echo "make rebuild      Clean and rebuild native cvs.exe"
	@echo "make run          Run native simulation and write output.csv"
	@echo "make web          Build WebAssembly output under build/web"
	@echo "make web-clean    Remove build/web"

.PHONY: all clean rebuild run plot compare-plot reference web web-clean help
