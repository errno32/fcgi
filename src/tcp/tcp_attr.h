struct tcp_attr 
{
	int sfd;
	int port;
	int service;	/* zrobić makra */
	pthread_t thread;

	int loop;
};
