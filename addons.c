#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "addons.h"

void printTitle() {
    const char *title = "PISKVORKY";
    const int width = 40; // Šírka rámu (možno prispôsobiť)
 
    printf("\n");
    for (int i = 0; i < width; i++) printf("="); // Horný rám
    printf("\n");
 
    int padding = (width - strlen(title)) / 2; // Výpočet odsadenia
 
    printf("|");
    for (int i = 1; i < width - 1; i++) {
        if (i == padding) {
            printf("%s", title);
            i += strlen(title) - 1; // Posuň index o dĺžku názvu
        } else {
            printf(" ");
        }
    }
    printf("|\n");
 
    for (int i = 0; i < width; i++) printf("="); // Dolný rám
    printf("\n");
}
 
void process_special_command(const char* input, Server* server) {
    if (strncmp(input, "-h", 2) == 0) {
        // Príkaz pre help
        printf("HELP: Zadajte riadok a stĺpec pre ťah, alebo použite príkazy:\n");
        printf("-h: zobraziť tento pomocník\n");
        printf("-e: odísť zo hry\n");
        printf("-s: uložiť hru \n");
    } else if (strncmp(input, "-e", 2) == 0) {
        // Príkaz pre leave
        printf("Ukončenie hry\n");
        exit(0); 
    } else if (strncmp(input, "-s", 2) == 0) {
        // Príkaz pre save
        printf("Zadajte názov súboru pre uloženie hry: ");
        char filename[100];
        scanf("%s", filename);
        save_game_to_file(server->game, filename);
    } else {
        // Ak sa nezadaný príkaz nenachádza, ignorujeme
        printf("Neznámy príkaz: %s\n", input);
    }
}
