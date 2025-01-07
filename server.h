#ifndef SERVER_H
#define SERVER_H

#include "game_logic.h"

// Spustenie servera
int start_server(IPCMethod method);

// Spracovanie hernej logiky na serveri
void process_game_logic(GameBoard *game);

#endif 
