#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>

#include "conf.h"
/* P_SIZE -- link :3 */

#include "src/rec/recorder.h"
#include "src/tcp/connector.h"
#include "src/fcgi/parser.h"

/******************************************************/
int rec_get_date(char *);
int rec_get_type(int, char *);
int rec_get_errstr(int, char *);
/* oraz REC() i REC_ERR() */

int open_tcp_socket	(int, int);	/* port, service */
int tcp_thread	(struct tcp_attr *);
int tcp_socket	(struct tcp_attr *);
int tcp_bind	(struct tcp_attr *);
int tcp_listen	(struct tcp_attr *);
int tcp_accept	(struct tcp_attr *);
int close_tcp_socket(struct tcp_attr *);

int fcgi_parse(char *, int);
int fcgi_asemble_body(struct fcgi_body *, int, char *);
int fcgi_get_len(char *, int *, int*);	/* buffer, length, position */

#include "src/rec/rec_get_date.c"
#include "src/rec/rec_get_type.c"
#include "src/rec/rec_get_errstr.c"

#include "src/tcp/open_tcp_socket.c"
#include "src/tcp/init_thread.c"
#include "src/tcp/socket.c"
#include "src/tcp/bind.c"
#include "src/tcp/listen.c"
#include "src/tcp/accept.c"
#include "src/tcp/close_tcp_socket.c"

#include "src/fcgi/parse.c"
#include "src/fcgi/asemble_body.c"
#include "src/fcgi/get_len.c"
/******************************************************/

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
