CC = g++
CFLAGS = -Wall -g -std=c++11

all: encrypt

encrypt: main.cpp
	$(CC) $(CFLAGS) main.cpp -o encrypt

clean:
	rm -rf encrypt *.dSYM