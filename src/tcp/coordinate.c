int tcp_coordinate(struct tcp_attr *tcp)
{
	record rec;

	/* przygotowanie struktury tcp_recived */
	if(tcp->recived == NULL)  
	{
		tcp->recived = malloc(sizeof(struct tcp_recived));

		if(tcp->recived == NULL)  
		{
			rec.error = errno;
			REC_ERR(ERROR, rec.error, "malloc() wewnątrz"
				"tcp_coordinate() / port=%d, service=%d",
				tcp->port,
				tcp->service);
			return 1;
		}
	}

	struct tcp_recived *rs = tcp->recived;	/* skrót  */

	rs->nfd = -1;
	rs->buffer = NULL;
	rs->buffer_len = 0;
	rs->buffer_data_len = 0;


	int res_accept = tcp_accept(tcp); /* funkcja blokująca */
	if(res_accept != 0) return 2;

	int res_recive = tcp_recive_all(tcp);
	if(res_recive != 0) return 3;

	fcgi_parse(tcp);


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
