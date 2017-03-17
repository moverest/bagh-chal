CC=gcc
SRC_DIR=src
BUILD_DIR=build

TERMBOX_FLAG=-ltermbox

all: test_graphics_tb

test_graphics_tb: build_dir
	$(CC) $(TERMBOX_FLAG) -o $(BUILD_DIR)/test_graphics_tb $(SRC_DIR)/test_graphics_tb.c $(SRC_DIR)/graphics_tb.c

build_dir:
	mkdir -p $(BUILD_DIR)

clean:
	rm -rvf $(BUILD_DIR)
