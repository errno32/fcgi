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

	struct tcp_recived *rs = malloc(sizeof(struct tcp_recived));

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

	tcp_recive_all(params, rs, nfd);

	fcgi_parse(rs->buffer, rs->buffer_data_len);


	//fcgi_send_test_page(nfd);
	
	/* params->loop = 0;  -- przerwanie pętli w init_thread() */
	 
	printf("\tWychodzę z tcp_accept()...\n");
	close(nfd);
	return 0;
}
