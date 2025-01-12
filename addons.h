#ifndef ADDONS_H
#define ADDONS_H
#include "shm.h"
#include "server.h"
#include "client.h"

void printTitle();
void process_special_command(const char* input, Server* server);

#endif
