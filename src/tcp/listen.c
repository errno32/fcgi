#include <sys/socket.h>

#define LQUEUE 20	/* liczba połączeń oczekujących na accept() */

/* Funkcja    : init_listen()
 * Opis       : kolejkuje nadchodzące połączenia przed wywołąniem systemowej
 		funkcji accepr()
 * Argumenty  : *params - wskaźnik na strukturę zawierającą wszystkie dane
 * Wynik      : 0 - sukces
 		1 - błąd funkcji listen()
 */

int tcp_listen(struct tcp_attr *params) 
{
	record rec;

	int result = 0;
	result = listen(params->sfd, LQUEUE);

	if(result != 0) 
	{
		rec.error = errno;
		REC_ERR(ERROR, rec.error, "Błąd wywołania listen()"
			" w funkcji tcp_listen() / port=%d, service=%d",
			params->service,
			params->port);

		return 1;
	}

	/*
	REC("listen() wykonany pomyślnie / port=%d, service=%d",
		params->port,
		params->service);
	*/

	return 0;
}
