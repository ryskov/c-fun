#include "../includes/std.h"

#include <unistd.h>
#include <netdb.h>

void prepare_hints(struct addrinfo *hints) {
	struct addrinfo ai;

	memset(&ai, 0, sizeof ai);
	ai.ai_family = AF_UNSPEC;
	ai.ai_socktype = SOCK_STREAM;
	ai.ai_flags = AI_PASSIVE;

	memset(hints, 0, sizeof *hints);
	*hints = ai;
}

void prepare_listening_socket(struct addrinfo *hints, const char *port, int *listener) {
	int rv;
	int yes = 1;

	struct addrinfo *p, *ai;

	if ((rv = getaddrinfo(NULL, port, hints, &ai)) != 0) {
		fprintf(stderr, "server: %s\n", gai_strerror(rv));
		exit(1);
	}

	for (p = ai; p != NULL; p = p->ai_next) {
		*listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (*listener < 0) {
			continue;
		}

		setsockopt(*listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

		if (bind(*listener, p->ai_addr, p->ai_addrlen) < 0) {
			close(*listener);
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		exit(2);
	}

	freeaddrinfo(ai);
}

void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}