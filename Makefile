all: server

server: dirs
	gcc -Wall networking/util.c server.c -o bin/server

dirs:
	mkdir -p bin 