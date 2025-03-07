#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "server.h"
#include "shm.h"


// Definícia názvu zdieľanej pamäte
#define SHM_NAME "/game_shared_memory_"

// Vytvorenie servera
Server* create_server(int size, int win_condition, int num_players, char *symbols, char names[10][50], int max_clients) {
    Server *server = malloc(sizeof(Server));
    server->game = create_game_logic(size, win_condition, num_players, symbols, names);
    server->max_clients = max_clients;
    server->connected_clients = 0;

 
    shm_init(SHM_NAME, server);
    int shm_fd;
    server_shm_open(SHM_NAME, server, &shm_fd);
    server->shm_fd_ = shm_fd;

    return server;
}

// Uvoľnenie pamäte servera
void destroy_server(Server *server) {
    server_shm_close(server->shm_fd_, server);
   
    shm_destroy(SHM_NAME);
    destroy_game_logic(server->game);
    free(server);
}

// Spracovanie ťahu klienta
void handle_client_move(Server *server, int row, int col) {
    
    char* player_name = server->game->player_names[server->game->current_player];
    

    if (make_move(server->game, row, col)) {
        printf("Hráč %s vykonal platný ťah na políčko (%d, %d).\n", player_name, row, col);

        if (check_winner(server->game)) {
            print_board(server->game);
            printf("Hráč %s vyhral hru!\n", player_name);
            notify_winner(server, player_name);
        } else {
            // Prepnúť na ďalšieho hráča
            server->game->current_player = (server->game->current_player + 1) % server->game->num_players; 
            send_game_state(server);         
        }
    } else {
        printf("Hráč %s vykonal neplatný ťah na políčko (%d, %d).\n", player_name, row, col);
    }
}

// Posielanie stavu hry všetkým klientom cez zdieľanú pamäť
void send_game_state(Server *server) {

    memcpy(server->shm_ptr, server->game, sizeof(GameLogic));
    print_board(server->game); 
}

// Notifikácia o víťazovi a zápis do zdieľanej pamäti
void notify_winner(Server *server, char *winner_name) {

    strncpy(server->game->winner, winner_name, sizeof(server->game->winner) - 1);
}
