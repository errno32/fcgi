/* Funkcja    : tcp_recive_all()
 * Opis       : Rezerwuje pamięć i odbiera dane od serwera.
 * Argumenty  : tcp_attr *tcp
 * 		tcp_recived *rs
 * 		int nfd
 * Wynik      : 0	- sukces
 * 		1	- błąd realloc() pod zmienną rs->buffer
 * 		2	- połączenie przerwane przez serwer
 * 		3	- błąd recv() inny niż EAGAIN lub EWOULDBLOCK
 */

int tcp_recive_all(struct tcp_attr *tcp) 
{
	record rec;

	struct tcp_recived *rs = tcp->recived;

	int recived = 0;	/* tyle odebrano przy ostatnim recv() */

	int allocating = 1;	/* czy alokować dodatkową pamięć? */
	int repeating = 1;	/* czy wykonywać dalej pętlę? */
	int waited = 0;		/* czy wykonało już spanko? */ 

	char *buffer_before;

	do {
		if(allocating) 
		{
			buffer_before = rs->buffer;
			rs->buffer_len += P_SIZE;
			rs->buffer = realloc(rs->buffer, rs->buffer_len);

			if(rs->buffer == NULL) 
			{
				rec.error = errno;
				REC_ERR(ERROR, rec.error, "realloc() wewnątrz"
					" tc_z_ufer() / port=%d, service=%d",
					tcp->port,
					tcp->service);

				if(buffer_before != NULL) free(buffer_before);

				return 1;
			}
		}

		recived = recv(rs->nfd,
			&(rs->buffer)[rs->buffer_len - P_SIZE],
			P_SIZE, MSG_DONTWAIT);

		if(recived == P_SIZE) 
		{
			/* dopełniono bufer -- kontynuacja pobierania */

			allocating	= 1;
			repeating	= 1; 
			waited		= 0;

			rs->buffer_data_len += recived;
		}
		else if(recived == 0)
		{
			REC_ERR(ERROR, rec.error, "Połączenie przerwane przez"
				" serwer / port=%d, service=%d",
				tcp->port,
				tcp->service);

			if(rs->buffer != NULL) 
			{
				free(rs->buffer);
				rs->buffer = NULL;
			}

			return 2; 
		}
		else if(recived == -1) 
		{
			rec.error = errno;
			if((rec.error == EAGAIN) || (rec.error == EWOULDBLOCK))
			{
				if(waited)  
				{
					allocating	= 0;
					repeating	= 0; 
					waited		= 1;

					recived = 0;
				}
				else 
				{
					REC_ERR(WARNING, rec.error,
					"Ponawiam odbiór za %d mikrosekund"
					" / port=%d, service=%d",
					NONBLOCK_WAITER,
					tcp->port,
					tcp->service);

					usleep(NONBLOCK_WAITER);

					allocating	= 0;
					repeating	= 1; 
					waited		= 1;

					recived = 0;
				}
			}
			else
			{
				/* niespodziewnay błąd */
				REC_ERR(ERROR, rec.error, "recv()"
					" / port=%d, service=%d",
					tcp->port,
					tcp->service);

				if(rs->buffer != NULL) 
				{
					free(rs->buffer);
					rs->buffer = NULL;
				}

				return 3;
			}
		}
		else 
		{
			/* pobrano dane bez przeszkód -- koniec pętli */
			rs->buffer_data_len = rs->buffer_len - P_SIZE + recived;
			repeating = 0;
		}

	} while(repeating);


	REC("Odebrano \033[32m%d B\033[0m (rez. %d B)"
		" / p=%d, s=%d",
		rs->buffer_data_len,
		rs->buffer_len,
		tcp->port,
		tcp->service);

	/* zapisanie sekwencji FCGI do pliku */
	/*
	FILE *rfd = fopen("_odp.bin", "wb+");
	if(rfd == NULL)
	{
		perror("Blad as75df87");
		return -1;
	}
	printf("\tZapisuję bufor do pliku...\n");
	fwrite(rs->buffer, rs->buffer_data_len, 1, rfd);
	fclose(rfd);
	*/

	return 0;
}
