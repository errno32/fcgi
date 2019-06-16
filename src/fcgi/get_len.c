/* Funkcja    : fcgi_get_len()
 * Opis       : 
 * Argumenty  : 
 * Wynik      : 
 */

int fcgi_get_len(char *buffer, int *length, int *position) 
{
	if(buffer[0] & (1 << 7))
	{
		*length = (unsigned char) ((buffer[0] & 0x7F) << 24)
			+ (unsigned char) (buffer[1] << 16)
			+ (unsigned char) (buffer[2] << 8)
			+ (unsigned char) buffer[3];
		*position += 4;
	}
	else 
	{
		*length = buffer[0];
		*position += 1;
	}

	return 0;
}
