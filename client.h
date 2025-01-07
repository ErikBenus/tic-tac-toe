#ifndef CLIENT_H
#define CLIENT_H

#include "game_logic.h"

// Pripojenie klienta k serveru
int connect_to_server(IPCMethod method);

// Spustenie hernej slučky pre klienta
void start_client();

#endif