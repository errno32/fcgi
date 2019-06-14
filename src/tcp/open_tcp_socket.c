/* Funkcja    : create_tcp_server()
 * Opis       : funkcja tworzy serwer TCP w oddzielnym wątku, którego
		przeznaczenie określa <int service>
 * Argumenty  : int port	- numer portu dla serwera
 		int service	- typ usługi 
 * Wynik      : 0 - sukces
 		1 - błąd alokacji pamięci (przerwanie działania)
 */

int create_tcp_server(int port, int service) 
{
	rec info;

	struct tcp_attr *params = NULL;
	params = malloc(sizeof(struct tcp_attr));

	if(params == NULL) 
	{
		info.err = errno;
		snprintf(info.str, REC_INFO_LEN, "Problem z alokacją pamięci"
			" wewnątrz funkcji create_tcp_server(int port=%d,"
			" int service=%d)", port, service);
		record(FUCK, info.err, info.str);
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
		(void *) init_thread,
		(void *) params);

	return 0;
}
