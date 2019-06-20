/* Funkcja    : close_tcp_socket()
 * Opis       : Zamyka port i czyści strukturę tcp_attr.
 * Argumenty  : *params - wskaźnik na strukturę zawierającą wszystkie dane
 * Wynik      : 0 - sukces
 		1 - argument wskazuje na NULL
 */

int close_tcp_socket(struct tcp_attr *params) 
{
	record rec; 

	if(params == NULL) 
	{
		REC_ERR(ERROR, 0, "close_tcp_socket() wywołane dla NULL"
			" / port=%d, service=%d",
			params->port,
			params->service);
		
		return 1;
	}

	/* zamknięcie gniazda siecioego */
	if(params->sfd != -1) close(params->sfd);

	/* oczekiwanie na zakończenie wątku */
	if(params->thread != 0) pthread_join(params->thread, NULL);

	/* zapisuję, żeby po zwolnieniu pamięci wyświetlić pełny komunikat */
	int info_service = params->service, info_port = params->port;
	
	free(params);
	params = NULL;

	REC("Zamknięto gniazdo TCP / port=%d, service=%d",
		info_port, info_service);
	
	return 0;
}
