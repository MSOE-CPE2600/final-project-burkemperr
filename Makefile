CC = gcc
CFLAGS = -Wall -g
TARGETS = server voter
SHM_KEY = 12345

all: $(TARGETS)

server: server.c
	$(CC) $(CFLAGS) -o server server.c

voter: voter.c
	$(CC) $(CFLAGS) -o voter voter.c

clean:
	rm -f $(TARGETS)
