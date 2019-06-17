int tcp_recive_all(struct tcp_attr *params, struct tcp_recived *rs, int nfd) 
{
	record rec;

	rs->buffer = NULL;
	rs->buffer_len = 0;
	rs->buffer_data_len = 0;

	char *buffer_before;
	int recived = 0;

	do {
		buffer_before = rs->buffer;
		rs->buffer_len += P_SIZE;
		rs->buffer = realloc(rs->buffer, rs->buffer_len);

		if(rs->buffer == NULL) 
		{
			rec.error = errno;
			REC_ERR(ERROR, rec.error, "realloc() wewnątrz"
				" tcp_accept() / port=%d, service=%d",
				params->port,
				params->service);

			if(buffer_before != NULL) free(buffer_before);

			close(nfd);
			return 1;
		}

		recived = recv(nfd, &(rs->buffer)[rs->buffer_len - P_SIZE],
			P_SIZE, 0);

		/*
		REC("Odebrano %d B, zaalokowano już łącznie %d B danych"
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
			 return 2; 
		}

	} while(recived == P_SIZE);

	rs->buffer_data_len = rs->buffer_len - P_SIZE + recived;

	REC("Odebrano \033[32m%d B\033[0m (rez. %d B)"
		" / p=%d, s=%d",
		rs->buffer_data_len,
		rs->buffer_len,
		params->port,
		params->service);

	/* zapisanie danych od NGINX */
	FILE *rfd = fopen("_odp.bin", "wb+");
	if(rfd == NULL)
	{
		perror("Blad as75df87");
		return -1;
	}

	printf("\tZapisuję bufor do pliku...\n");
	fwrite(rs->buffer, rs->buffer_data_len, 1, rfd);
	fclose(rfd);

	return 0;
}
