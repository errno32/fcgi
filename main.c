#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>

#include "conf.h"

#include "src/rec/recorder.h"
/* TODO zastąpić /record rec/ strukturą w paramsach
 * REC_ERR(int type, int errno, const char *format, ...)
 * REC(const char *format, ...)
 * int rec_get_date(char *);
 * int rec_get_type(int, char *);
 * int rec_get_errstr(int, char *);
 * */

#include "src/tcp/connector.h"
/* int open_tcp_socket	(int, int);	/ port, service /
 * int tcp_thread	(struct tcp_attr *);
 * int tcp_socket	(struct tcp_attr *);
 * int tcp_bind	(struct tcp_attr *);
 * int tcp_listen	(struct tcp_attr *);
 * int tcp_accept	(struct tcp_attr *);
 * int close_tcp_socket(struct tcp_attr *);
 * */


#include "src/fcgi/parse.c"

#include "src/fcgi/asemble_body.c"
#include "src/fcgi/get_len.c"
/* fcgi_parse() */


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
