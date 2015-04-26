all: server server2


server: dirs
	gcc -Wall networking/util.c server.c -o bin/server

server2: dirs
	gcc -Wall networking/util.c server2.c -o bin/server2

dirs:
	mkdir -p bin 