/* Funkcja    : tcp_thread()
 * Opis       : Funkcja rozpoczyna pracę nowego wątku (ID: tcp->thread)
 * 		i wykonuje procedurę otwarcia nowego gniazda TCP.
 * Argumenty  : *tcp	- wskaźnik na strukturę tcp_sttr
 * Wynik      : 0	- sukces
 *		1	- w funkcji tcp_socket, _bind lub _listen wystąpił błąd
 */

int tcp_thread(struct tcp_attr *tcp) 
{
	record rec;
	int res_bind, res_listen;

	tcp_socket(tcp);
	if(tcp->sfd != -1) 
	{
		res_bind = tcp_bind(tcp);
		if(res_bind == 0) 
		{
			res_listen = tcp_listen(tcp);
			if(res_listen == 0) 
			{
				REC("\033[32mGniazdo otwarte pomyślnie!\033[0m"
					" / port=%d, service=%d",
					tcp->port,
					tcp->service);

				tcp->loop = 1;

				while(tcp->loop) 
				{	
					tcp_coordinate(tcp);
				}
				
				close_tcp_socket(tcp);
				return 0;
			}
		}
	}

	REC_ERR(FUCK, 0, "Nie udało się stworzyć gniazda TCP!"
		" / port=%d, service=%d",
		tcp->port,
		tcp->service);

	close_tcp_socket(tcp);
	return 1;
}
