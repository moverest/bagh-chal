
SRC_DIR = src
BUILD_DIR = build

CCFLAGS = -std=gnu99 -O2
CC = gcc $(CCFLAGS)
LD_FLAGS = -lm

TERMBOX_LD_FLAGS=-ltermbox
SDL_LD_FLAGS=-lSDL2 -lSDL2_ttf

.PHONY: all
all: $(BUILD_DIR) $(foreach f, test_graphics_tb test_game test_test main_tb test_graphics_minimalist_sdl main_minimalist_sdl test_menu_tb test_menu_graphics_sdl test_matrix test_neuralnet, $(BUILD_DIR)/$f)

.PHONY: debug
debug: CCFLAGS += -DDEBUG -g -Wall
debug: all

$(BUILD_DIR)/main_tb: LD_FLAGS += $(TERMBOX_LD_FLAGS)
$(BUILD_DIR)/main_tb: $(BUILD_DIR) $(foreach f, graphics_tb.o ui_game.o ui_game_menu.o game.o models.o ai_rand.o menu.o ui_menu.o ui_main.o ui_end_menu.o ui_pause_menu.o stack.o ai_heuristic.o ai_simple_heuristic.o, $(BUILD_DIR)/$f)

$(BUILD_DIR)/main_minimalist_sdl: LD_FLAGS += $(SDL_LD_FLAGS)
$(BUILD_DIR)/main_minimalist_sdl: $(BUILD_DIR) $(foreach f, graphics_minimalist_sdl.o ui_game.o ui_game_menu.o game.o models.o ai_rand.o menu.o ui_menu.o ui_main.o ui_end_menu.o ui_pause_menu.o stack.o ai_heuristic.o ai_simple_heuristic.o, $(BUILD_DIR)/$f)


$(BUILD_DIR)/test_test: $(BUILD_DIR) $(BUILD_DIR)/test.o
$(BUILD_DIR)/test_game: $(BUILD_DIR) $(foreach f, models.o test.o game.o ai_rand.o stack.o, $(BUILD_DIR)/$f)
$(BUILD_DIR)/test_matrix: $(BUILD_DIR) $(foreach f, matrix.o test.o, $(BUILD_DIR)/$f)
$(BUILD_DIR)/test_stack: $(BUILD_DIR) $(foreach f, stack.o test.o, $(BUILD_DIR)/$f)
$(BUILD_DIR)/test_neuralnet: $(BUILD_DIR) $(foreach f, neuralnet.o matrix.o test.o randn.o, $(BUILD_DIR)/$f)

$(BUILD_DIR)/test_graphics_tb: LD_FLAGS += $(TERMBOX_LD_FLAGS)
$(BUILD_DIR)/test_graphics_tb: $(BUILD_DIR) $(foreach f, models.o graphics_tb.o graphics_test.o test_graphics_tb.o, $(BUILD_DIR)/$f)

$(BUILD_DIR)/test_graphics_minimalist_sdl: LD_FLAGS += $(SDL_LD_FLAGS)
$(BUILD_DIR)/test_graphics_minimalist_sdl: $(BUILD_DIR) $(foreach f, models.o graphics_minimalist_sdl.o graphics_test.o, $(BUILD_DIR)/$f)

$(BUILD_DIR)/test_menu_tb: LD_FLAGS += $(TERMBOX_LD_FLAGS)
$(BUILD_DIR)/test_menu_tb: $(BUILD_DIR) $(foreach f, models.o menu.o ui_menu.o graphics_tb.o menu_test.o, $(BUILD_DIR)/$f)

$(BUILD_DIR)/test_menu_graphics_sdl: LD_FLAGS += $(SDL_LD_FLAGS)
$(BUILD_DIR)/test_menu_graphics_sdl: $(BUILD_DIR) $(foreach f, models.o menu.o ui_menu.o graphics_minimalist_sdl.o menu_test.o, $(BUILD_DIR)/$f)

$(BUILD_DIR)/%: $(BUILD_DIR)/%.o
	$(CC) -o $@ $(filter %.o, $^) $(LD_FLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: clean
clean:
	rm -rvf $(BUILD_DIR)
