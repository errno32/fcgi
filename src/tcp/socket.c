#include <sys/socket.h>	/* socket() */
#include <netdb.h>	/* getprotobyname() */

/* Funkcja    : tcp_socket()
 * Opis       : tworzy deskryptor gniazda TCP
 * Argumenty  : *params - wskaźnik na strukturę zawierającą wszystkie dane
 * Wynik      : 0 - success
 		1 - błąd funkcji socket()

 * Func       :
 * Desc       :
 * Args       :
 * Return     :
 	
 */

int tcp_socket(struct tcp_attr *params) 
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

	params->sfd = socket(AF_INET, SOCK_STREAM, protocol);

	if(params->sfd == -1) 
	{	
		rec.error = errno;
		REC_ERR(ERROR, rec.error, "Nie udało się utworzyć gniazda"
			" funkcją tcp_socket() / port=%d, service=%d",
			params->port, params->service);
		return 1;
	}

	/*
	REC("socket() wykonany pomyślnie / port=%d, service=%d",
		params->port,
		params->service);
	*/

	return 0;
}
