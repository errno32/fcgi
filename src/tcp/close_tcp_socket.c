/* Funkcja    : close_tcp_socket()
 * Opis       : Zamyka port i czyści strukturę tcp_attr.
 * Argumenty  : *tcp - wskaźnik na strukturę zawierającą wszystkie dane
 * Wynik      : 0 - sukces
 *		1 - argument wskazuje na NULL
 */

int close_tcp_socket(struct tcp_attr *tcp) 
{
	record rec; 

	if(tcp == NULL) 
	{
		REC_ERR(ERROR, 0, "close_tcp_socket() wywołane dla NULL"
			" / port=%d, service=%d",
			tcp->port,
			tcp->service);
		
		return 1;
	}

	/* zamknięcie gniazda siecioego */
	if(tcp->sfd != -1) close(tcp->sfd);

	/* oczekiwanie na zakończenie wątku */
	if(tcp->thread != 0) pthread_join(tcp->thread, NULL);

	/* zapisuję, żeby po zwolnieniu pamięci wyświetlić pełny komunikat */
	int info_service = tcp->service, info_port = tcp->port;
	
	free(tcp);
	tcp = NULL;

	REC("Zamknięto gniazdo TCP / port=%d, service=%d",
		info_port, info_service);
	
	return 0;
}
