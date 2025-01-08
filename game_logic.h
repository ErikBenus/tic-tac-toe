#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

typedef struct GameLogic {
    int size;                // Veľkosť hracej plochy
    int win_condition;       // Počet políčok potrebných na výhru
    int num_players;         // Počet hráčov
    int current_player;      // Index aktuálneho hráča
    char player_symbols[10]; // Symboly hráčov (maximálne 10 hráčov)
    char player_names[10][50]; // Mená hráčov (maximálne 10 hráčov, max. dĺžka mena 50 znakov)
    char **board;            // Hracia plocha
    char winner[50];
} GameLogic;

GameLogic* create_game_logic(int size, int win_condition, int num_players, char *symbols, char names[10][50]);
void destroy_game_logic(GameLogic *game);
void print_board(GameLogic *game);
int make_move(GameLogic *game, int row, int col);
int check_winner(GameLogic *game);

#endif