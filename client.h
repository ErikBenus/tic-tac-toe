#ifndef CLIENT_H
#define CLIENT_H

#include "game_logic.h"

typedef struct GameConfig{
    int num_players;
    int board_size;
    char names[10][50];
    char symbols[10];
    int win_condition;
} GameConfig;

GameConfig setup_game();
void join_game();
void save_game_to_file(GameLogic *game, const char *filename);

#endif