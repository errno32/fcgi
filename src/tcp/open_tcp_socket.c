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

	struct tcp_attr *params = NULL;
	params = malloc(sizeof(struct tcp_attr));

	if(params == NULL) 
	{
		rec.error = errno;
		REC_ERR(FUCK, rec.error,
			"malloc() wewnątrz open_tcp_socket()"
			" / port=%d, service=%d",
			port,
			service);
		return 1;
	}

	params->sfd	= -1;
	params->port	= port;
	params->service	= service;
	params->thread	= 0;
	params->loop	= 0;

	pthread_create(
		&(params->thread),
		NULL,
		(void *) tcp_thread, /* tcp_thread(params) */
		(void *) params);

	return 0;
}
