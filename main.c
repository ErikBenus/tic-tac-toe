#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include "client.h"
#include "server.h"

// Mutex na synchronizáciu prístupu k zdieľanej pamäti
pthread_mutex_t game_mutex;

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

// Funkcia pre server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include "client.h"
#include "server.h"

// Mutex na synchronizáciu prístupu k zdieľanej pamäti
pthread_mutex_t game_mutex;

// Funkcia pre server
void* server_thread(void* arg) {
    GameConfig config = *((GameConfig*)arg);
    printf("Spúšťam hru pre %d hráčov na hracom poli %dx%d.\n", 
            config.num_players, config.board_size, config.board_size);

    // Vytvorenie servera a pripojenie klienta
    Server *server = create_server(config.board_size, 3, config.num_players, config.symbols, config.names, 10);
    print_board(server->game);
    // Synchronizácia prístupu k zdieľanej pamäti
    pthread_mutex_lock(&game_mutex);
    join_game(); // Klient sa pripojí k serveru
    pthread_mutex_unlock(&game_mutex);

    // Hlavná slučka pre spracovanie ťahov

    int row, col;
    while (1) {
        // Simulujeme príchod ťahu od klienta

        printf("%s, zadajte riadok a stĺpec pre ťah: ", server->game->player_names[server->game->current_player]);
        scanf("%d %d", &row, &col);

        pthread_mutex_lock(&game_mutex);
        handle_client_move(server, row, col);  // Spracovanie ťahu klienta
        pthread_mutex_unlock(&game_mutex);

        // Pre kontrolu vyhrávajúcej podmienky by mal server ešte skontrolovať, či nie je víťaz
        if (check_winner(server->game)) {
            break;
        }
    }

    // Uvoľnenie servera po ukončení hry
    pthread_mutex_lock(&game_mutex);
    destroy_server(server);  // Uvoľnenie servera
    pthread_mutex_unlock(&game_mutex);

    return NULL;
}

// Funkcia pre klienta
void* client_thread(void* arg) {

    return NULL;
}

int main() {
    int choice;

    // Inicializujeme mutex
    if (pthread_mutex_init(&game_mutex, NULL) != 0) {
        perror("Nepodarilo sa inicializovať mutex.");
        return 1;
    }

    printTitle();
    printf("Vitajte v hre Piškvorky pre viacerých hráčov!\n");
    printf("1. Nová hra\n");
    printf("2. Načítať hru zo súboru\n");
    printf("3. Ukončiť aplikáciu\n");
    printf("Vyberte možnosť: ");
    scanf("%d", &choice);

    while (choice != 1 && choice != 2 && choice != 3) {
        printf("Neplatná voľba! Skúste znova.\n");
        scanf("%d", &choice);
    }

    if (choice == 1) {
        GameConfig config = setup_game();
        pthread_t server_tid, client_tid;

        // Vytvorenie a spustenie serverového vlákna
        pthread_create(&server_tid, NULL, server_thread, &config);
        // Vytvorenie a spustenie klientského vlákna
        pthread_create(&client_tid, NULL, client_thread, NULL);

        // Čakáme na dokončenie oboch vlákien
        pthread_join(server_tid, NULL);
        pthread_join(client_tid, NULL);

    } else if (choice == 2) {
        // Implementácia načítania hry zo súboru
        printf("Načítanie hry zo súboru ešte nie je implementované.\n");
    } else {
        printf("Ukončujem aplikáciu.\n");
    }

    // Uvoľnenie mutexu
    pthread_mutex_destroy(&game_mutex);

    return 0;
}
