#include <stdio.h>
#include <stdlib.h>
#include "game_logic.h"
#include <string.h>

// Vytvorenie hernej logiky
GameLogic* create_game_logic(int size, int win_condition, int num_players, char *symbols, char names[10][50]) {
    GameLogic *game = malloc(sizeof(GameLogic));
    game->size = size;
    game->win_condition = win_condition;
    game->num_players = num_players;
    game->current_player = 0;

    // Kopírovanie symbolov a mien hráčov
    for (int i = 0; i < num_players; i++) {
        game->player_symbols[i] = symbols[i];
        strcpy(game->player_names[i], names[i]); // Skopírovanie mena
    }

    // Inicializácia hracej plochy
    game->board = malloc((size) * sizeof(char *)); 
    for (int i = 0; i <= size; i++) {
        game->board[i] = malloc((size ) * sizeof(char)); 
        for (int j = 0; j <= size; j++) {
            game->board[i][j] = '_'; // Prvé riadky/stĺpce sú na číslovanie
        }
    }

    // Číslovanie riadkov a stĺpcov

    return game;
}


// Uvoľnenie pamäte
void destroy_game_logic(GameLogic *game) {
    for (int i = 0; i <= game->size; i++) {
        free(game->board[i]);
    }
    free(game->board);
    free(game);
}

// Vykreslenie hracej plochy
void print_board(GameLogic *game) {
    int cislovanieR = 0;
    int cislovanieS = 0;

    printf("  ");
    for(int i = 0; i < game->size; i++) {
        cislovanieS++;
        if(i < 9) {
            printf("  %d", cislovanieS);
        } else {
            printf(" %d", cislovanieS);
        }
        
    }

    printf("\n");

    for (int i = 0; i <= game->size; i++) {
        cislovanieR++;
        if(i < 9) {
            printf(" %d  ", cislovanieR);
        } else {
            printf("%d  ", cislovanieR);
        }
        
        for (int j = 0; j <= game->size; j++) {
            printf("%c  ", game->board[i][j]);  
        }
        printf("\n");
    }
}

// Vykonanie ťahu
int make_move(GameLogic *game, int row, int col) {
    // Posun o 1 kvôli číslovaniu
    if (row < 1 || row > game->size || col < 1 || col > game->size || game->board[row][col] != '-') {
        return 0; // Neplatný ťah
    }
    game->board[row][col] = game->player_symbols[game->current_player];
    return 1; // Platný ťah
}

// Kontrola víťaza
int check_winner(GameLogic *game) {
    int size = game->size;
    int win = game->win_condition;
    char current_symbol = game->player_symbols[game->current_player];

    // Kontrola riadkov, stĺpcov a diagonál
    for (int i = 1; i <= size; i++) {
        for (int j = 1; j <= size; j++) {
            if (game->board[i][j] == current_symbol) {
                // Horizontálne
                if (j + win - 1 <= size) {
                    int count = 0;
                    for (int k = 0; k < win; k++) {
                        if (game->board[i][j + k] == current_symbol) count++;
                    }
                    if (count == win) return 1;
                }

                // Vertikálne
                if (i + win - 1 <= size) {
                    int count = 0;
                    for (int k = 0; k < win; k++) {
                        if (game->board[i + k][j] == current_symbol) count++;
                    }
                    if (count == win) return 1;
                }

                // Diagonálne (doprava dole)
                if (i + win - 1 <= size && j + win - 1 <= size) {
                    int count = 0;
                    for (int k = 0; k < win; k++) {
                        if (game->board[i + k][j + k] == current_symbol) count++;
                    }
                    if (count == win) return 1;
                }

                // Diagonálne (doprava hore)
                if (i - win + 1 >= 1 && j + win - 1 <= size) {
                    int count = 0;
                    for (int k = 0; k < win; k++) {
                        if (game->board[i - k][j + k] == current_symbol) count++;
                    }
                    if (count == win) return 1;
                }
            }
        }
    }
    return 0;
}
