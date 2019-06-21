/* Funkcja    : tcp_accept()
 * Opis       : Obsługuje połączenia. Ignoruje obce IP.
 * Argumenty  : *tcp - wskaźnik na strukturę tcp_attr
 * Wynik      : 0	- sukces
 *		1	- dostęp z nieuprawnionego hosta
 *		2	- błąd funkcji accept()
 */
int tcp_accept(struct tcp_attr *tcp)
{
	record rec;

	struct in_addr localhost;
	inet_aton("127.0.0.1", &localhost);

	struct sockaddr_in sender;
	socklen_t sender_len = sizeof(struct sockaddr);

	tcp->recived->nfd
		= accept(tcp->sfd, (struct sockaddr *) &sender, &sender_len);

	tcp->clocks = clock();

	/* obsługa błędów */
	if(tcp->recived->nfd == -1) 
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
		close(tcp->recived->nfd);
		return 1;
		/* TODO zwiększenie puli akceptowalnych adresów */
	}

	REC("Port %d / Nowe połączenie (nfd=%d)", tcp->port, tcp->recived->nfd);
	return 0;
}
