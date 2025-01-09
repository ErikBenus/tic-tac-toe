# Nastavenie cieľa
TARGET_SERVER := server
TARGET_CLIENT := client
TARGET := piskvorky

# Objektové súbory pre server a client
SERVER_OBJS := server.o common.o
CLIENT_OBJS := client.o common.o
TARGET_OBJS := main.o $(SERVER_OBJS) $(CLIENT_OBJS)

# Kompilátor a príkazy
CC := gcc
CFLAGS += -std=c99 -O3 -Wall -Wextra -Wpedantic

# Cieľ pre server
server: $(SERVER_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# Cieľ pre client
client: $(CLIENT_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# Predvolený cieľ (vytvorenie hlavného programu)
all: $(TARGET)

$(TARGET): $(TARGET_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# Vyčistenie generovaných súborov
clean:
	$(RM) -f *.o $(TARGET) $(TARGET_SERVER) $(TARGET_CLIENT)

# Pravidlo pre kompiláciu objektových súborov
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

.PHONY: all clean server client