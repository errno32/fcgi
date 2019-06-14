int rec_get_type(int type, char *str) 
{
	if(str == NULL) 
	{
		fprintf(stderr, "Wskaźnik NULL w funkcji rec_get_type()\n");
		return 1;
	}

	switch(type)
	{
		case OK:
		snprintf(str, REC_TYPE_SZ, "\033[32mOK\033[0m");
		break;
	
		case WARNING:
		snprintf(str, REC_TYPE_SZ, "\033[33mWARNING\033[0m");
		break;

		case ERROR:
		snprintf(str, REC_TYPE_SZ, "\033[31mERROR\033[0m");
		break;

		case FUCK:
		snprintf(str, REC_TYPE_SZ, "\033[41mFATAL ERROR\033[0m");
		break;

		default:
		snprintf(str, REC_TYPE_SZ, "\033[36m(undefined type)\033[0m");
	}

	return 0;
}
