#include "../includes/std.h"

#include <unistd.h>
#include <netdb.h>

void prepare_hints(struct addrinfo *hints);

int prepare_listening_socket(struct addrinfo *hints, const char *port, int *listener);

int select_read(int fdmax, fd_set *read_fds);

void *get_in_addr(struct sockaddr *sa);