#include "../includes/std.h"

#include <unistd.h>
#include <netdb.h>

void prepare_hints(struct addrinfo *hints);

void prepare_listening_socket(struct addrinfo *hints, const char *port, int *listener);

void *get_in_addr(struct sockaddr *sa);