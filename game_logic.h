#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#define BOARD_SIZE 3

typedef struct {
    char board[BOARD_SIZE][BOARD_SIZE];
    int turn; // 0 = X, 1 = O
} GameState;

// Inicializuje hru.
void initialize_game(GameState *game);

// Skontroluje, či má niekto výhru.
int check_winner(GameState *game);

// Vykoná ťah. Vracia 1, ak je úspešný, inak 0.
int make_move(GameState *game, int x, int y);

#endif
