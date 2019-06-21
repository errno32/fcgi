/* Funkcja    : tcp_socket()
 * Opis       : Tworzy deskryptor gniazda TCP.
 * Argumenty  : *tcp - wskaźnik na strukturę tcp_attr
 * Wynik      : 0	- success
 * 		1	- błąd funkcji socket()
 */

int tcp_socket(struct tcp_attr *tcp) 
{
	record rec;
	int protocol = 0;
	
	struct protoent *proto_struct = getprotobyname("tcp\0");
	if(proto_struct == NULL) 
	{
		REC_ERR(WARNING, 0, "getprotobyname() wewnątrz funkcji"
			" init_socket() zwróciło NULL. Ustawiam wartość"
			" domyślną: %d.", protocol);
	}
	else protocol = proto_struct->p_proto;

	tcp->sfd = socket(AF_INET, SOCK_STREAM, protocol);

	if(tcp->sfd == -1) 
	{	
		rec.error = errno;
		REC_ERR(ERROR, rec.error, "Nie udało się utworzyć gniazda"
			" funkcją tcp_socket() / port=%d, service=%d",
			tcp->port, tcp->service);
		return 1;
	}

	/*
	REC("socket() wykonany pomyślnie / port=%d, service=%d",
		tcp->port,
		tcp->service);
	*/

	return 0;
}
