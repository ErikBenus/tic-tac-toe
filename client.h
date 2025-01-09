#ifndef CLIENT_H
#define CLIENT_H

/*#include "game_logic.h"

// Pripojenie klienta k serveru
int connect_to_server(IPCMethod method);

// Spustenie hernej slučky pre klienta
void start_client();*/


typedef struct GameConfig{
    int num_players;
    int board_size;
    char names[10][50];
    char symbols[10];
} GameConfig;

GameConfig setup_game();
void join_game();

#endif