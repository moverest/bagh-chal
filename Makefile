CCFLAGS=-std=gnu99
SRC_DIR=src
BUILD_DIR=build
CC=gcc $(CCFLAGS)

TERMBOX_FLAG=-ltermbox

debug: CCFLAGS += -DDEBUG -g -Wall
debug: all

all: $(foreach f, test_graphics_tb test_game test_test, $(BUILD_DIR)/$f)

$(BUILD_DIR)/test_game: $(BUILD_DIR) $(foreach f, models.o test.o game.o, $(BUILD_DIR)/$f)
	$(CC) -o $@ $(SRC_DIR)/test_game.c $(foreach f, models.o test.o game.o, $(BUILD_DIR)/$f)

$(BUILD_DIR)/test_graphics_tb: $(BUILD_DIR) $(foreach f, models.o graphics_tb.o graphics_test.o, $(BUILD_DIR)/$f)
	$(CC) $(TERMBOX_FLAG) -o $@ $(SRC_DIR)/test_graphics_tb.c $(foreach f, models.o graphics_tb.o graphics_test.o, $(BUILD_DIR)/$f)

$(BUILD_DIR)/graphics_tb.o: $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/graphics_tb.c -o $@

$(BUILD_DIR)/game.o: $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/game.c -o $@

$(BUILD_DIR)/models.o: $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/models.c -o $@

$(BUILD_DIR)/test.o: $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/test.c -o $@

$(BUILD_DIR)/graphics_test.o: $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/graphics_test.c -o $@

$(BUILD_DIR)/ui.o: $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/ui.c -o $@

$(BUILD_DIR)/test_test: $(BUILD_DIR) $(BUILD_DIR)/test.o
	$(CC) $(BUILD_DIR)/test.o $(SRC_DIR)/test_test.c -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rvf $(BUILD_DIR)
