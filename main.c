#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <semaphore.h> 

#include "client.h"
#include "server.h"
#include "addons.h"

 
 
void run_game(GameConfig config, sem_t *game_semaphore) {
    printf("Spúšťam hru pre %d hráčov na hracom poli %dx%d.\n",
            config.num_players, config.board_size, config.board_size);

    Server *server = create_server(config.board_size, config.win_condition, config.num_players, config.symbols, config.names, 10);
    print_board(server->game);

    char input[100];
    getchar();
    int row, col;
    while (1) {
        memset(input, 0, sizeof(input));

        printf("%s, zadajte riadok a stĺpec pre ťah: ", server->game->player_names[server->game->current_player]);

        fgets(input, sizeof(input), stdin);

        input[strcspn(input, "\n")] = 0;

        //ak sa nic nezada, program ziada znova o vstup
        if (input[0] == '\0') {
            continue;  
        }


        if (input[0] == '-') {
            process_special_command(input);
            continue;
        }

        sscanf(input, "%d %d", &row, &col);

        sem_wait(game_semaphore);
        handle_client_move(server, row, col);
        sem_post(game_semaphore);

        if (check_winner(server->game)) {
            break;
        }
    }

    sem_wait(game_semaphore);
    destroy_server(server);
    sem_post(game_semaphore);
}

int main() {
    int choice;
    sem_unlink("/game_semaphore");

    sem_t *game_semaphore = sem_open("/game_semaphore", O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);

    if (game_semaphore == SEM_FAILED) {
        perror("Nepodarilo sa inicializovať semafor.");
        exit(EXIT_FAILURE);
    }

    printTitle();
    printf("Vitajte v hre Piškvorky pre viacerých hráčov!\n");
    printf("1. Nová hra\n");
    printf("2. Načítať hru zo súboru\n");
    printf("3. Ukončiť aplikáciu\n");
    printf("Vyberte možnosť: ");
    scanf("%d", &choice);
    getchar();  // Vyprázdniť vstupný buffer

    while (choice != 1 && choice != 2 && choice != 3) {
        printf("Neplatná voľba! Skúste znova.\n");
        scanf("%d", &choice);
        getchar();
    }

    if (choice == 1) {
        GameConfig config = setup_game();
        run_game(config, game_semaphore);
    } else if (choice == 2) {
        printf("Načítanie hry zo súboru ešte nie je implementované.\n");
    } else {
        printf("Ukončujem aplikáciu.\n");
    }

    sem_close(game_semaphore);
    sem_unlink("/game_semaphore");

    return 0;
}