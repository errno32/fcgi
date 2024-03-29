#define LQUEUE 20	/* liczba połączeń oczekujących na accept() */

/* Funkcja    : tcp_listen()
 * Opis       : Kolejkuje nadchodzące połączenia przed wywołąniem systemowej
 *		funkcji accept().
 * Argumenty  : *tcp	- wskaźnik na strukturę zawierającą wszystkie dane
 * Wynik      : 0	- sukces
 *		1	- błąd funkcji listen()
 */

int tcp_listen(struct tcp_attr *tcp) 
{
	record rec;

	int result = 0;
	result = listen(tcp->sfd, LQUEUE);

	if(result != 0) 
	{
		rec.error = errno;
		REC_ERR(ERROR, rec.error, "Błąd wywołania listen()"
			" w funkcji tcp_listen() / port=%d, service=%d",
			tcp->service,
			tcp->port);

		return 1;
	}

	/*
	REC("listen() wykonany pomyślnie / port=%d, service=%d",
		tcp->port,
		tcp->service);
	*/

	return 0;
}
