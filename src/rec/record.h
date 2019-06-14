#define OK	0
#define WARNING	1
#define ERROR	2
#define FUCK	3

#define REC_DATE_SZ	64	/* rec_get_date() */
#define REC_TYPE_SZ	32	/* rec_get_type() */
#define REC_ERRN_SZ	128	/* rec_get_errstr() */

typedef struct Record 
{
	int error;
	char  err[REC_ERRN_SZ];
	char date[REC_DATE_SZ];
	char type[REC_TYPE_SZ];
} record;

#include "rec_get_date.c"
#include "rec_get_type.c"
#include "rec_get_errstr.c"

#define REC(_format, ...) \
{ \
	rec_get_date(rec.date); \
	fprintf(stdout, "%s " _format "\n", \
	rec.date, __VA_ARGS__); \
}

#define REC_ERR(_type, _error, _format, ...) \
{ \
	rec_get_date(rec.date); \
	rec_get_type(_type, rec.type); \
	rec_get_errstr(_error, rec.err); \
	fprintf(stderr, "%s %s / %s / " _format "\n", \
	rec.date, rec.type, rec.err, __VA_ARGS__); \
}

