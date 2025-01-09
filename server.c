#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "server.h"

// Definícia názvu zdieľanej pamäte
#define SHM_NAME "/game_shared_memory"

// Vytvorenie servera
Server* create_server(int size, int win_condition, int num_players, char *symbols, char names[10][50], int max_clients) {
    Server *server = malloc(sizeof(Server));
    server->game = create_game_logic(size, win_condition, num_players, symbols, names);
    server->max_clients = max_clients;
    server->connected_clients = 0;

    // Vytvorenie zdieľanej pamäti pre hru
    int shm_fd = shm_open(SHM_NAME, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR); // Vytvoriť zdieľanú pamäť
    if (shm_fd == -1) {
        perror("Nemôžem vytvoriť zdieľanú pamäť");
        exit(EXIT_FAILURE);
    }

    // Nastavenie veľkosti zdieľanej pamäte
    if (ftruncate(shm_fd, sizeof(GameLogic)) == -1) {
        perror("Nemôžem nastaviť veľkosť zdieľanej pamäti");
        close(shm_fd);
        exit(EXIT_FAILURE);
    }

    // Mapovanie zdieľanej pamäti do adresného priestoru
    server->shm_ptr = mmap(NULL, sizeof(GameLogic), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (server->shm_ptr == MAP_FAILED) {
        perror("Nemôžem mapovať zdieľanú pamäť");
        close(shm_fd);
        exit(EXIT_FAILURE);
    }

    close(shm_fd);
    return server;
}

// Uvoľnenie pamäte servera
void destroy_server(Server *server) {
    // Zrušiť zdieľanú pamäť
    if (munmap(server->shm_ptr, sizeof(GameLogic)) == -1) {
        perror("Nemôžem odmapovať zdieľanú pamäť");
    }

    // Odstrániť zdieľanú pamäť
    if (shm_unlink(SHM_NAME) == -1) {
        perror("Nemôžem odstrániť zdieľanú pamäť");
    }

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
    // Uloženie aktuálneho stavu hry do zdieľanej pamäti
    memcpy(server->shm_ptr, server->game, sizeof(GameLogic));
    print_board(server->game); // Taktiež zobrazenie stavu na serveri (v reálnom prípade sa posiela klientom cez sockety)
}

// Notifikácia o víťazovi a zápis do zdieľanej pamäti
void notify_winner(Server *server, char *winner_name) {

    // Zápis informácie o víťazovi do zdieľanej pamäti
    strncpy(server->game->winner, winner_name, sizeof(server->game->winner) - 1);
}
