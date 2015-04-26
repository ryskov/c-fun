#include "networking/util.h"

int main(void) {
	struct addrinfo hints;

	int listener;

	prepare_hints(&hints);

	prepare_listening_socket(&hints, "9999", &listener);

	if (listen(listener, 10 == -1)) {
		perror("listen");
		exit(3);
	}

	return 0;
}