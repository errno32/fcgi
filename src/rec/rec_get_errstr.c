// string.h

int rec_get_errstr(int err, char *str) 
{
	/* sukces, nie ma potrzeby wyswietlania czegokolwiek */
	if(str == NULL) 
	{
		printf("Wskaźnik NULL w rec_get_errstr()\n");
		return 1;
	}

	strerror_r(err, str, REC_ERRN_SZ);
	
	return 0;
}
