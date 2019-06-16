/* Funkcja    : 
 * Opis       : 
 * Argumenty  : 
 * Wynik      : 
 */

int fcgi_asemble_body(struct fcgi_body *body, int length, char *buffer)
{
	if(length == 0) 
	{
		body->done = 1;
		return 0;
	}

	body->len += length; 

	char *buffer_old = body->buffer;
	body->buffer = realloc(body->buffer, body->len);

	if(body->buffer == NULL) 
	{
		printf("błąd, wszystko wyczyścić\n");
		free(buffer_old);
		return 1;
	}

	memcpy(&(body->buffer[body->len - length]), buffer, length);
 	
	return 0;
}
