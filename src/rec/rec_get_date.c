// time.h

int rec_get_date(char *str) 
{
	time_t t = time(NULL);
	struct tm *date = localtime(&t);
/*	struct tm
	{
		int tm_sec;          seconds,  range 0 to 59         
		int tm_min;          minutes, range 0 to 59           
		int tm_hour;         hours, range 0 to 23            
		int tm_mday;         day of the month, range 1 to 31
		int tm_mon;          month, range 0 to 11           
		int tm_year;         The number of years since 1900 
		int tm_wday;         day of the week, range 0 to 6  
		int tm_yday;         day in the year, range 0 to 365
		int tm_isdst;        daylight saving time          
	};
*/
	if(str == NULL) 
	{
		fprintf(stderr, "Wskaźnik NULL w funkcji rec_get_date()\n");
		return 1;
	}

	snprintf(str,
		REC_DATE_SZ,
		"%02d:%02d:%02d %02d/%02d/%04d",
		date->tm_hour,
		date->tm_min,
		date->tm_sec,
		date->tm_mday,
		date->tm_mon+1,
		date->tm_year+1900);
	
	return 0;
}
