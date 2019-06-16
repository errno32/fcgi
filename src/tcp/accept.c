#include <sys/socket.h>	/* accept() */

#define RECV_LIMIT 4096	/* $ getconf PAGE_SIZE */
			/* sysconf(_SC_PAGESIZE) */
			/* byle nie większe niż limit recv() */

/* Funkcja    : tcp_accept()
 * Opis       : obsługuje nadchodzące połączenia
 * Argumenty  : *params - wskaźnik na strukturę zawierającą wszystkie dane
 * Wynik      : 0 - sukces
 		1 - błąd funkcji accept()
		2 -
 */
int tcp_accept(struct tcp_attr *params)
{
	record rec;
	int nfd = 0;

	struct in_addr localhost;
	inet_aton("127.0.0.1", &localhost);

	struct sockaddr_in sender;
	socklen_t sender_len = sizeof(struct sockaddr);

	nfd = accept(params->sfd, (struct sockaddr *) &sender, &sender_len);

	if(nfd == -1) 
	{
		rec.error = errno;
		REC_ERR(ERROR, rec.error, "accept() wewnątrz tcp_accept()"
			" / port=%d, service=%d",
			params->port,
			params->service);
		return 1;
	}

	/*
	printf("-- nfd, %s:%d\n",
		inet_ntoa(sender.sin_addr),
		ntohs(sender.sin_port));
	*/

	if(memcmp(&localhost, &(sender.sin_addr), sizeof(struct in_addr))) 
	{
		/* zewnętrzny adres IP */
		REC_ERR(WARNING, 0, "Próba połączenia z zewnętrznym serwerem"
			" / p=%d, s=%d", params->port, params->service);
		close(nfd);
		return 0;
	}

	REC("Port %d / Nowe połączenie (nfd=%d)", params->port, nfd);

	char *buffer = NULL, *buffer_before;	/* TODO free() */
	int recived = 0, buffer_len = 0;

	do {
		buffer_before = buffer;
		buffer_len += RECV_LIMIT;
		buffer = realloc(buffer, buffer_len);

		if(buffer == NULL) 
		{
			rec.error = errno;
			REC_ERR(ERROR, rec.error, "realloc() wewnątrz"
				" tcp_accept() / port=%d, service=%d",
				params->port,
				params->service);

			if(buffer_before != NULL) free(buffer_before);

			close(nfd);
			return 2;
		}

		recived = recv(nfd, &buffer[buffer_len - RECV_LIMIT],
			RECV_LIMIT, 0);

		/*
		REC("Odebrano %d B, zaalokowano łącznie %d B danych"
			" / port=%d, service=%d",
			recived,
			buffer_len,
			params->port,
			params->service);
		*/

		/* TODO sprawdzenie ostatniego rekordu na wypadek
		   dokładnego wypełnienia bloków */

		if(recived == 0) return 3;
		/* przerwanie połączenia */

	} while(recived == RECV_LIMIT);
	
	REC("Odebrano \033[32m%d B\033[0m (rez. %d B) od 127.0.0.1"
		" / p=%d, s=%d",
		buffer_len - (RECV_LIMIT - recived),
		buffer_len,
		params->port,
		params->service);

	//fcgi_parse(buffer, buffer_len - (RECV_LIMIT - recived));


	//fcgi_send_test_page(nfd);
	
	/* params->loop = 0;  -- przerwanie pętli w init_thread() */

	 
	printf("\tWychodzę z tcp_accept()...\n");
	close(nfd);
	return 0;
}
