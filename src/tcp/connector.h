/*
int open_tcp_socket	(int, int);	/port, service/
int tcp_thread	(struct tcp_attr *);
int tcp_socket	(struct tcp_attr *);
int tcp_bind	(struct tcp_attr *);
int tcp_listen	(struct tcp_attr *);
int tcp_accept	(struct tcp_attr *);
int close_tcp_socket(struct tcp_attr *);
*/

/* BIND_SLEEPER zadeklarowany w pliku conf.h */

struct tcp_attr 
{
	int sfd;
	int port;
	int service;	/* zrobić makra */
	pthread_t thread;

	int loop;
};
