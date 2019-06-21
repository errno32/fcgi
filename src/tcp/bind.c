/* Funkcja    : init_bind()
 * Opis       : Otwiera port dla gniazda; jeśli port jest zajęty, ponawia
 *		operację co BIND_SLEEPER sekund aż do skutku.
 * Argumenty  : *tcp	- wskaźnik na strukturę tcp_attr
 * Wynik      : 0	- sukces
 *		1	- błąd funkcji bind() -- inny niż EADDRINUSE
 */

int tcp_bind(struct tcp_attr *tcp) 
{
	record rec;
	const char localhost[] = "127.0.0.1\0";	/* połączenie lokalne */

	/* stworzenie i zainicjalizowanie struktury adresowej */
	struct sockaddr_in address;
	memset(&address, '\0', sizeof(struct sockaddr_in));

	/* wypełnienie struktury odpowiednimi właściwościami */
	address.sin_family = AF_INET;			/* IPv4 family */
	address.sin_port = htons(tcp->port);		/* port */
	inet_aton(localhost, &(address.sin_addr));	/* adres IP */

	int result = 0;
	do {
		result = bind(tcp->sfd, (struct sockaddr *) &address,
				sizeof(struct sockaddr));

		if(result != 0) 
		{
			rec.error = errno;
			if(rec.error == EADDRINUSE) 
			{
				REC_ERR(WARNING, rec.error,
					"Port %d dla usługo %d jest zajęty."
					" Ponawiam za %d sekund...",
					tcp->port,
					tcp->service,
					BIND_SLEEPER);

				sleep(BIND_SLEEPER);
			}
			else 
			{
				REC_ERR(ERROR, rec.error,
					"Błąd wywołania bind() w funkcji"
					" tcp_bind() / port = %d, service = %d",
					tcp->service,
					tcp->port);
				
				return 1;
			}
		}

	} while(result != 0);

	/*
	REC("bind() wykonany pomyślnie / port=%d, service=%d",
		tcp->port,
		tcp->service);
	*/
	
	return 0;
}
