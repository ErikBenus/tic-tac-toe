#ifndef SHM_H
#define SHM_H

#include "game_logic.h"



typedef struct Server {
    GameLogic *game;       // Ukazovateľ na hernú logiku
    void *shm_ptr;         // Ukazovateľ na zdieľanú pamäť
    int max_clients;       // Maximálny počet klientov
    int connected_clients; // Počet pripojených klientov
    int shm_fd_;
} Server;


void shm_init(char *name, Server* server);
void shm_destroy(char *name);
void server_shm_open(char *name, Server* server, int *out_fd_shm);
void server_shm_close(int shm_fd, Server* server);

#endif