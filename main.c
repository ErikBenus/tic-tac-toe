#include <stdio.h>
#include <stdlib.h>
#include "client.h"
#include "server.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>


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



int main() {
    int choice;

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
        printf("Spúšťam hru pre %d hráčov na hracom poli %dx%d.\n", 
                config.num_players, config.board_size, config.board_size);
        
      
    } else if (choice == 2) {
        
    } else {
        printf("Ukončujem aplikáciu.\n");
    }

    return 0;
}
