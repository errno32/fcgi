#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>

#include "conf.h"

#include "src/rec/record.h"

#include "src/tcp/tcp_attr.h"
int open_tcp_socket	(int, int);	/* port, service */
	int tcp_thread	(struct tcp_attr *);
	int tcp_socket	(struct tcp_attr *);
	int tcp_bind	(struct tcp_attr *);
	int tcp_listen	(struct tcp_attr *);
	int tcp_accept	(struct tcp_attr *);	/* obsługa połączeń */
	int close_tcp_socket(struct tcp_attr *);

#include "src/tcp/open_tcp_socket.c"
	#include "src/tcp/init_thread.c"
	#include "src/tcp/socket.c"
	#include "src/tcp/bind.c"
	#include "src/tcp/listen.c"
	#include "src/tcp/accept.c"
	#include "src/tcp/close_tcp_socket.c"

int main(void)
{
	record rec; /* niezbędne dla funkcji REC() i REC_ERR() */
	REC("Rozpoczęcie pracy programu.%c", 0);

	open_tcp_socket(12345, 1);


	/* przytrzymywadełko / inkrementacja, bo kompilator */
	int znak = fgetc(stdin); znak++;
	

	REC("Zakończenie pracy programu.%c", 0);
	return 0;
}	

/* REC_ERR(WARNING, 0, "Przykładowy błąd na porcie %d", 1234); */



