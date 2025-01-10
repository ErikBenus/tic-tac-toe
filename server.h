

#ifndef SERVER_H
#define SERVER_H

#include "game_logic.h"
#include "shm.h"

// Definícia názvu zdieľanej pamäte
#define SHM_NAME "/game_shared_memory"



// Funkcie na prácu so serverom
Server* create_server(int size, int win_condition, int num_players, char *symbols, char names[10][50], int max_clients);
void destroy_server(Server *server);
void handle_client_move(Server *server, int row, int col);
void send_game_state(Server *server);
void notify_winner(Server *server, char *winner_name);

#endif

