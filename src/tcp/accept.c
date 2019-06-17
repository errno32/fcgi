#include <sys/socket.h>	/* accept() */

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

	/* punkt blokowania funkcji */
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

	/* zabezpieczenie przed niepowołanym dostępem */
	if(memcmp(&localhost, &(sender.sin_addr), sizeof(struct in_addr))) 
	{
		REC_ERR(WARNING, 0, "Próba połączenia z %s:%d / p=%d, s=%d",
			inet_ntoa(sender.sin_addr),
			ntohs(sender.sin_port),
			params->port,
			params->service);
		close(nfd);
		return 0;
		/* TODO zwiększenie puli akceptowalnych adresów */
	}

	REC("Port %d / Nowe połączenie (nfd=%d)", params->port, nfd);

	char *buffer = NULL, *buffer_before;	/* TODO free() */
	int recived = 0, buffer_len = 0;

	do {
		buffer_before = buffer;
		buffer_len += P_SIZE;
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

		recived = recv(nfd, &buffer[buffer_len - P_SIZE],
			P_SIZE, 0);

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

		if(recived == 0)
		{
			REC_ERR(ERROR, rec.error, "Połączenie przerwane przez"
				" serwer / port=%d, service=%d",
				params->port,
				params->service);
			 return 3; 
		}

	} while(recived == P_SIZE);
	
	REC("Odebrano \033[32m%d B\033[0m (rez. %d B)"
		" / p=%d, s=%d",
		buffer_len - (P_SIZE - recived),
		buffer_len,
		params->port,
		params->service);

	/* przesył danych */
	fcgi_parse(buffer, buffer_len - (P_SIZE - recived));


	//fcgi_send_test_page(nfd);
	
	/* params->loop = 0;  -- przerwanie pętli w init_thread() */
	 
	printf("\tWychodzę z tcp_accept()...\n");
	close(nfd);
	return 0;
}
