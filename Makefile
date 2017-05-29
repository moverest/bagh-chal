CCFLAGS=-std=gnu99 -lm
SRC_DIR=src
BUILD_DIR=build
CC=gcc $(CCFLAGS)

TERMBOX_FLAG=-ltermbox
SDL_FLAG=-lSDL2 -lSDL2_ttf

all: $(foreach f, test_graphics_tb test_game test_test main_tb test_graphics_minimalist_sdl main_minimalist_sdl test_menu_tb test_menu_graphics_sdl test_matrix test_neuralnet, $(BUILD_DIR)/$f)

debug: CCFLAGS += -DDEBUG -g -Wall
debug: all

all: $(foreach f, test_graphics_tb test_game test_test main_tb test_graphics_minimalist_sdl main_minimalist_sdl test_menu_tb test_menu_graphics_sdl test_matrix test_neuralnet test_stack, $(BUILD_DIR)/$f)

$(BUILD_DIR)/test_game: $(BUILD_DIR) $(foreach f, models.o test.o game.o ai_rand.o stack.o, $(BUILD_DIR)/$f)
	$(CC) -o $@ $(SRC_DIR)/test_game.c $(foreach f, models.o test.o game.o ai_rand.o stack.o, $(BUILD_DIR)/$f)

$(BUILD_DIR)/test_matrix: $(BUILD_DIR) $(foreach f, matrix.o test.o, $(BUILD_DIR)/$f)
	$(CC) -o $@ $(SRC_DIR)/test_matrix.c $(foreach f, matrix.o test.o, $(BUILD_DIR)/$f)

$(BUILD_DIR)/test_stack: $(BUILD_DIR) $(foreach f, stack.o test.o, $(BUILD_DIR)/$f)
	$(CC) -o $@ $(SRC_DIR)/test_stack.c $(foreach f, stack.o test.o, $(BUILD_DIR)/$f)

$(BUILD_DIR)/test_neuralnet: $(BUILD_DIR) $(foreach f, neuralnet.o matrix.o test.o randn.o, $(BUILD_DIR)/$f)
	$(CC) -o $@ $(SRC_DIR)/test_neuralnet.c $(foreach f, neuralnet.o matrix.o test.o randn.o, $(BUILD_DIR)/$f)

$(BUILD_DIR)/test_graphics_tb: $(BUILD_DIR) $(foreach f, models.o graphics_tb.o graphics_test.o, $(BUILD_DIR)/$f)
	$(CC) $(TERMBOX_FLAG) -o $@ $(SRC_DIR)/test_graphics_tb.c $(foreach f, models.o graphics_tb.o graphics_test.o, $(BUILD_DIR)/$f)

$(BUILD_DIR)/test_graphics_minimalist_sdl: $(BUILD_DIR) $(foreach f, models.o graphics_minimalist_sdl.o graphics_test.o, $(BUILD_DIR)/$f)
	$(CC) $(SDL_FLAG) -o $@ $(SRC_DIR)/test_graphics_minimalist_sdl.c $(foreach f, models.o graphics_minimalist_sdl.o graphics_test.o, $(BUILD_DIR)/$f)

$(BUILD_DIR)/test_menu_tb: $(BUILD_DIR) $(foreach f, models.o menu.o ui_menu.o graphics_tb.o menu_test.o, $(BUILD_DIR)/$f)
	$(CC) $(TERMBOX_FLAG) -o $@ $(SRC_DIR)/test_menu_tb.c $(foreach f, models.o menu.o ui_menu.o graphics_tb.o menu_test.o, $(BUILD_DIR)/$f)

$(BUILD_DIR)/test_menu_graphics_sdl: $(BUILD_DIR) $(foreach f, models.o menu.o ui_menu.o graphics_minimalist_sdl.o menu_test.o, $(BUILD_DIR)/$f)
	$(CC) $(SDL_FLAG) -o $@ $(SRC_DIR)/test_menu_graphics_sdl.c $(foreach f, models.o menu.o ui_menu.o graphics_minimalist_sdl.o menu_test.o, $(BUILD_DIR)/$f)

$(BUILD_DIR)/main_tb: $(BUILD_DIR) $(foreach f, graphics_tb.o ui_game.o ui_game_menu.o game.o models.o ai_rand.o menu.o ui_menu.o ui_main.o ui_end_menu.o ui_pause_menu.o stack.o  ai_heuristic.o ai_simple_heuristic.o, $(BUILD_DIR)/$f)
	$(CC) $(TERMBOX_FLAG) $(SRC_DIR)/main_tb.c  $(foreach f, graphics_tb.o ui_game.o ui_game_menu.o game.o models.o ai_rand.o menu.o ui_menu.o ui_main.o ui_end_menu.o ui_pause_menu.o stack.o ai_heuristic.o ai_simple_heuristic.o, $(BUILD_DIR)/$f) -o $@

$(BUILD_DIR)/main_minimalist_sdl: $(BUILD_DIR) $(foreach f, graphics_minimalist_sdl.o ui_game.o ui_game_menu.o game.o models.o ai_rand.o menu.o ui_menu.o ui_main.o ui_end_menu.o ui_pause_menu.o stack.o ai_heuristic.o ai_simple_heuristic.o, $(BUILD_DIR)/$f)
	$(CC) $(SDL_FLAG) $(SRC_DIR)/main_minimalist_sdl.c  $(foreach f, graphics_minimalist_sdl.o ui_game.o ui_game_menu.o game.o models.o ai_rand.o menu.o ui_menu.o ui_main.o ui_end_menu.o ui_pause_menu.o stack.o ai_heuristic.o ai_simple_heuristic.o, $(BUILD_DIR)/$f) -o $@

$(BUILD_DIR)/test_test: $(BUILD_DIR) $(BUILD_DIR)/test.o
	$(CC) $(BUILD_DIR)/test.o $(SRC_DIR)/test_test.c -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rvf $(BUILD_DIR)
