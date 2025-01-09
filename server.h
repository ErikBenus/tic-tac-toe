

#ifndef SERVER_H
#define SERVER_H

#include "game_logic.h"

// Definícia názvu zdieľanej pamäte
#define SHM_NAME "/game_shared_memory"

// Štruktúra pre server
typedef struct Server {
    GameLogic *game;       // Ukazovateľ na hernú logiku
    void *shm_ptr;         // Ukazovateľ na zdieľanú pamäť
    int max_clients;       // Maximálny počet klientov
    int connected_clients; // Počet pripojených klientov
} Server;

// Funkcie na prácu so serverom
Server* create_server(int size, int win_condition, int num_players, char *symbols, char names[10][50], int max_clients);
void destroy_server(Server *server);
void handle_client_move(Server *server, int row, int col);
void send_game_state(Server *server);
void notify_winner(Server *server, char *winner_name);

#endif

