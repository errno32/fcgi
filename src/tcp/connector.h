struct tcp_attr 
{
	int sfd;
	int port;
	int service;	/* zrobić makra */
	pthread_t thread;

	int loop;
};

int open_tcp_socket	(int, int);	/* port, service */
int tcp_thread	(struct tcp_attr *);
int tcp_socket	(struct tcp_attr *);
int tcp_bind	(struct tcp_attr *);
int tcp_listen	(struct tcp_attr *);
int tcp_accept	(struct tcp_attr *);
int close_tcp_socket(struct tcp_attr *);

#include "open_tcp_socket.c"
#include "init_thread.c"
#include "socket.c"
#include "bind.c"
#include "listen.c"
#include "accept.c"
#include "close_tcp_socket.c"
