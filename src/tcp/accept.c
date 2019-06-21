/* Funkcja    : tcp_accept()
 * Opis       : Obsługuje połączenia. Ignoruje obce IP.
 * Argumenty  : *tcp - wskaźnik na strukturę tcp_attr
 * Wynik      : 0	- sukces
 *		1	- błąd alokacji pamięci pod strukturę tcp_recived
 *		2	- błąd funkcji accept()
 */
int tcp_accept(struct tcp_attr *tcp)
{
	record rec;

	/* przygotowanie danych przed nawiązaniem połączenia */
	struct in_addr localhost;
	inet_aton("127.0.0.1", &localhost);

	struct sockaddr_in sender;
	socklen_t sender_len = sizeof(struct sockaddr);

	struct tcp_recived *rs = malloc(sizeof(struct tcp_recived));
	if(rs == NULL)  
	{
		rec.error = errno;
		REC_ERR(ERROR, rec.error, "malloc() wewnątrz tcp_accept()"
			" / port=%d, service=%d",
			tcp->port,
			tcp->service);
		return 1;
	}
	tcp->recived = rs;

	rs->nfd = -1;
	rs->buffer = NULL;
	rs->buffer_len = 0;
	rs->buffer_data_len = 0;


	/* punkt blokowania funkcji */
	rs->nfd = accept(tcp->sfd, (struct sockaddr *) &sender, &sender_len);
	tcp->clocks = clock();

	/* obsługa błędów */
	if(rs->nfd == -1) 
	{
		rec.error = errno;
		REC_ERR(ERROR, rec.error, "accept() wewnątrz tcp_accept()"
			" / port=%d, service=%d",
			tcp->port,
			tcp->service);
		return 2;
	}

	/* zabezpieczenie przed niepowołanym dostępem */
	if(memcmp(&localhost, &(sender.sin_addr), sizeof(struct in_addr))) 
	{
		REC_ERR(WARNING, 0, "Próba połączenia z %s:%d / p=%d, s=%d",
			inet_ntoa(sender.sin_addr),
			ntohs(sender.sin_port),
			tcp->port,
			tcp->service);
		close(rs->nfd);
		return 0;
		/* TODO zwiększenie puli akceptowalnych adresów */
	}

	REC("Port %d / Nowe połączenie (nfd=%d)", tcp->port, rs->nfd);


	tcp_recive_all(tcp);


	
	fcgi_parse(tcp, rs->buffer, rs->buffer_data_len);



	fcgi_send_test_page(rs->nfd);
	
	/* tcp->loop = 0;  -- przerwanie pętli w init_thread() */
	 
	close(rs->nfd);

	clock_t finish_clocks = clock();
	clock_t used_clocks = finish_clocks - tcp->clocks;

	REC("Operacja trwała  %ld taktów procesora, czyli %f s (%ld pps)"
		" / port=%d, service=%d",
		used_clocks,
		(double) used_clocks / CLOCKS_PER_SEC,
		CLOCKS_PER_SEC / used_clocks,
		tcp->port,
		tcp->service);

	return 0;
}
