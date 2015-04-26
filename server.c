#include "includes/sock_inet.h"
#include "includes/std.h"

#include "networking/util.h"

#define PORT "1338"
#define BACKLOG 10

int main (void) {
	fd_set master;
	fd_set read_fds;
	int fdmax;

	int listener;
	int newfd;
	struct sockaddr_storage remoteaddr;
	socklen_t addrlen;

	char buf[256];
	int nbytes;

	char remoteIP[INET6_ADDRSTRLEN];

	int i, j;

	struct addrinfo hints;

	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	prepare_hints(&hints);

	prepare_listening_socket(&hints, PORT, &listener);

	if (listen(listener, BACKLOG) == -1) {
		perror("listen");
		exit(3);
	}

	FD_SET(listener, &master);

	fdmax = listener;

	for (;;) {
		read_fds = master;
		if (select_read(fdmax+1, &read_fds) != 0) {
			continue;
		}

		for (i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &read_fds)) {
				if (i == listener) {
					addrlen = sizeof remoteaddr;
					newfd = accept(listener,
						(struct sockaddr *)&remoteaddr,
						&addrlen);

					if (newfd == -1) {
						perror("accept");
					}
					else {
						FD_SET(newfd, &master);
						if (newfd > fdmax) {
							fdmax = newfd;
						}

						printf("server: new connection from %s on socket %d\n",
							inet_ntop(remoteaddr.ss_family,
								get_in_addr((struct sockaddr*)&remoteaddr),
								remoteIP, INET6_ADDRSTRLEN),
							newfd);
					}
				}
				else {
					if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
						if (nbytes == 0) {
							printf("server: socket %d hung up\n", i);
						}
						else {
							perror("recv");
						}
						close(i);
						FD_CLR(i, &master);	
					}
					else {
						for (j = 0; j <= fdmax; j++) {
							if (FD_ISSET(j, &master)) {
								if (j != listener && j != i) {
									if (send(j, buf, nbytes, 0) == -1) {
										perror("send");
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return 0;
}