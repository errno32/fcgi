/* Funkcja    : init_thread()
 * Opis       : funkcja rozpoczynająca pracę nowego wątku (ID params->thread),
		porządkująca działanie pozostałych funkcji z rodzinki (poza
		create_ i close_ )
 * Argumenty  : *params - wskaźnik na strukturę zawierającą wszystkie dane
 * Wynik      : 0 - sukces
 		1 - w funkcji init_socket, _bind lub _listen wystąpił błąd
 */

int tcp_thread(struct tcp_attr *params) 
{
	record rec;
	int res_bind, res_listen;

	tcp_socket(params);
	if(params->sfd != -1) 
	{
		res_bind = tcp_bind(params);
		if(res_bind == 0) 
		{
			res_listen = tcp_listen(params);
			if(res_listen == 0) 
			{
				REC("\033[32mGniazdo otwarte pomyślnie!\033[0m"
					" / port=%d, service=%d",
					params->port,
					params->service);

				params->loop = 1;

				while(params->loop) 
				{	
					tcp_accept(params);
				}
				
				close_tcp_socket(params);
				return 0;
			}
		}
	}

	REC_ERR(FUCK, 0, "Nie udało się stworzyć gniazda TCP!"
		" / port=%d, service=%d",
		params->port,
		params->service);

	close_tcp_socket(params);
	return 1;
}
