/* Funkcja    : open_tcp_socket()
 * Opis       : Alokuje w pamięci strukturę tcp_sttr i uruchamia procedurę
 * 		w nowym wątku.
 * Argumenty  : port	- unikatowy numer portu do wykorzystania
 * 		sercive	- typ usługi / TODO przypisać im makra
 * Wynik      : 0	- sukces
 * 		1	- błąd funkcji malloc()
 */

int open_tcp_socket(int port, int service) 
{
	record rec;

	struct tcp_attr *tcp = NULL;
	tcp = malloc(sizeof(struct tcp_attr));

	if(tcp == NULL) 
	{
		rec.error = errno;
		REC_ERR(FUCK, rec.error,
			"malloc() wewnątrz open_tcp_socket()"
			" / port=%d, service=%d",
			port,
			service);
		return 1;
	}

	tcp->sfd	= -1;
	tcp->port	= port;
	tcp->service	= service;
	tcp->thread	= 0;
	tcp->loop	= 0;
	tcp->recived	= NULL;

	pthread_create(
		&(tcp->thread),
		NULL,
		(void *) tcp_thread, /* tcp_thread(tcp) */
		(void *) tcp);

	return 0;
}
