CC=gcc
SRC_DIR=src
BUILD_DIR=build

TERMBOX_FLAG=-ltermbox

all: build_dir term

term:
	$(CC) $(TERMBOX_FLAG) -o $(BUILD_DIR)/test $(SRC_DIR)/term_main.c $(SRC_DIR)/graphics_tb.c

build_dir:
	mkdir -p $(BUILD_DIR)

clean:
	rm -rvf $(BUILD_DIR)
