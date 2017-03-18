CCFLAGS=
SRC_DIR=src
BUILD_DIR=build
CC=gcc $(CCFLAGS)

TERMBOX_FLAG=-ltermbox

debug: CCFLAGS += -DDEBUG -g
debug: all

all: $(BUILD_DIR)/test_graphics_tb $(BUILD_DIR)/test_game $(BUILD_DIR)/test_test

$(BUILD_DIR)/test_game: build_dir $(BUILD_DIR)/models.o $(BUILD_DIR)/test.o
	$(CC) -o $@ $(SRC_DIR)/test_game.c $(SRC_DIR)/game.c $(BUILD_DIR)/models.o $(BUILD_DIR)/test.o

$(BUILD_DIR)/test_graphics_tb: build_dir $(BUILD_DIR)/models.o $(BUILD_DIR)/test.o
	$(CC) $(TERMBOX_FLAG) -o $@ $(SRC_DIR)/test_graphics_tb.c $(SRC_DIR)/graphics_tb.c $(BUILD_DIR)/models.o $(BUILD_DIR)/test.o

$(BUILD_DIR)/models.o: build_dir
	$(CC) -c $(SRC_DIR)/models.c -o $@

$(BUILD_DIR)/test.o: build_dir
	$(CC) -c $(SRC_DIR)/test.c -o $@

$(BUILD_DIR)/test_test: build_dir $(BUILD_DIR)/test.o
	$(CC) $(BUILD_DIR)/test.o $(SRC_DIR)/test_test.c -o $@

build_dir:
	mkdir -p $(BUILD_DIR)

clean:
	rm -rvf $(BUILD_DIR)
