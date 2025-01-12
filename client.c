#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "game_logic.h"


#define SHM_NAME "/game_shared_memory"


GameConfig setup_game() {
    GameConfig config;

    //zadanie poctu hracov pre hru
    printf("Zadajte počet hráčov (2-10): ");
    scanf("%d", &config.num_players);
    while (config.num_players < 2 || config.num_players > 10) {
        printf("Neplatný počet hráčov! Zadajte hodnotu medzi 2 a 10: ");
        scanf("%d", &config.num_players);
    }

    //zadanie velkosti hracieho pola
    printf("Zadajte veľkosť hracieho poľa (napr. 3 pre 3x3): ");
    scanf("%d", &config.board_size);
    while (config.board_size < 3 || config.board_size > 100) {
        printf("Neplatná veľkosť! Zadajte hodnotu medzi 3 a 100: ");
        scanf("%d", &config.board_size);
    }

    //pocet policok ptorenych na vitazstvo
    printf("Zadajte počet políčok potrebných na víťazstvo (min.3 a max.%d): ", config.board_size);
    scanf("%d", &config.win_condition);
    while (config.win_condition < 3 || config.win_condition > config.board_size) {
        printf("Neplatná veľkosť! Zadajte hodnotu medzi 3 a %d)", config.board_size);
        scanf("%d", &config.win_condition);
    }

    //zadanie mien jednotnotlivych hracov a ich symbolov
    for (int i = 0; i < config.num_players; i++) {
        printf("Zadajte meno hráča %d: ", i + 1);
        scanf("%s", config.names[i]);

        printf("Zadajte symbol pre hráča %s (jeden znak): ", config.names[i]);
        scanf(" %c", &config.symbols[i]);

        for (int j = 0; j < i; j++) {
            while (config.symbols[i] == config.symbols[j]) {
                printf("Symbol '%c' je už obsadený! Zadajte iný symbol: ", config.symbols[i]);
                scanf(" %c", &config.symbols[i]);
            }
        }
    }
    return config;
}

// Funkcia na pripojenie k hre 
void join_game() {
    int shm_fd;
    void *shm_ptr;

    // Otvorenie zdieľanej pamäti
    shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Nemôžem otvoriť zdieľanú pamäť");
        exit(1);
    }

    // Mapovanie zdieľanej pamäti do adresného priestoru
    shm_ptr = mmap(NULL, sizeof(GameLogic), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("Nemôžem mapovať zdieľanú pamäť");
        close(shm_fd);
        exit(1);
    }

    // Uzatvorenie zdieľanej pamäti
    munmap(shm_ptr, sizeof(GameLogic));
    close(shm_fd);
}


void save_game_to_file(GameLogic *game, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Nepodarilo sa otvoriť súbor na zápis");
        return;
    }

    // Zápis základných údajov o hre
    fprintf(file, "%d %d %d %d\n", game->size, game->win_condition, game->num_players, game->current_player);

    // Zápis hráčov (mená a symboly)
    for (int i = 0; i < game->num_players; i++) {
        fprintf(file, "%s %c\n", game->player_names[i], game->player_symbols[i]);
    }

    // Zápis herneho pola
    for (int i = 0; i < game->size; i++) {
        
        for (int j = 0; j < game->size; j++) {
            fprintf(file, "%2c ", game->board[i][j]); 
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Hra bola úspešne uložená do súboru '%s'.\n", filename);
}