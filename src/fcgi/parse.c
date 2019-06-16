#define FCGI_HEADER_LEN	8
#define FCGI_VERSION	1

#define FCGI_BEGIN_REQUEST       1
#define FCGI_ABORT_REQUEST       2
#define FCGI_END_REQUEST         3
#define FCGI_PARAMS              4
#define FCGI_STDIN               5
#define FCGI_STDOUT              6
#define FCGI_STDERR              7
#define FCGI_DATA                8
#define FCGI_GET_VALUES          9
#define FCGI_GET_VALUES_RESULT  10
#define FCGI_UNKNOWN_TYPE       11

struct fcgi_header
{
    unsigned char version;
    unsigned char type;
    unsigned char ID_B1;
    unsigned char ID_B0;
    unsigned char len_B1;
    unsigned char len_B0;
    unsigned char padding;
    unsigned char reserved;

    int ID;
    int len;
};

struct fcgi_body 
{
	char *buffer;
	int len;
	char done;
};

struct fcgi_recv  
{
	char *buffer;

	struct fcgi_body *params;
	struct fcgi_body *input;
};

/* prototypu do usunięcia XXX */
int fcgi_asemble_body(struct fcgi_body *, int, char *);
int fcgi_get_len(char *, int *, int*);	/* buffer, length, position */

int fcgi_parse(char *buffer, int len) 
{
	printf("funkcja fcgi_parse(char *buffer, int len == %d)\n", len);

	struct fcgi_recv *recived = NULL;
	recived = malloc(sizeof(struct fcgi_recv));

	if(recived == NULL) 
	{
		printf("problem z alokacjiom\n");
		return 3;
	}

	recived->buffer	= buffer;
	recived->input	= malloc(sizeof(struct fcgi_body));
	recived->params	= malloc(sizeof(struct fcgi_body));

	if((recived->input == NULL) || (recived->params == NULL)) 
	{
		printf("problem z alokacją\n");
		return 1;	/* zwolnic tez recived() */
	}

	recived->input->buffer	= NULL;
	recived->input->len	= 0;
	recived->input->done	= 0;

	recived->params->buffer	= NULL;
	recived->params->len	= 0;
	recived->params->done	= 0;


	int position = 0;
	while((position + FCGI_HEADER_LEN) <= len) 
	{
		/* bezpiecznie od buffer[position] do buffer[position + 7] */

		struct fcgi_header header;

		header.version	= buffer[position];
		header.type	= buffer[position+1];
		header.ID_B1	= buffer[position+2];
		header.ID_B0	= buffer[position+3];
		header.len_B1	= buffer[position+4];
		header.len_B0	= buffer[position+5];
		header.padding	= buffer[position+6];
		header.reserved	= buffer[position+7];

		if(header.version != FCGI_VERSION) 
		{
			printf("nieznana wersja protokołu FastCGI\n");
			return 1;
		}

		header.ID = 0;
		header.ID = (unsigned char) header.ID_B1;
		header.ID <<= 8;
		header.ID += (unsigned char) header.ID_B0;

		header.len = 0;
		header.len = (unsigned char) header.len_B1;
		header.len <<= 8;
		header.len += (unsigned char) header.len_B0;

		switch(header.type) 
		{
			case FCGI_BEGIN_REQUEST:
			/*printf("FCGI_BEGIN_REQUEST\t(8+%d B, "
			"position %d)\n", header.len, position);*/
			break;

			case FCGI_ABORT_REQUEST:
			printf("\tFCGI_ABORT_REQUEST\n");
			break;
			//case FCGI_END_REQUEST:

			case FCGI_PARAMS:
			/*printf("FCGI_PARAMS\t(8+%d B, position %d)\n",
			header.len, position);*/
			fcgi_asemble_body(recived->params, header.len,
			&(buffer[position + FCGI_HEADER_LEN]));
			break;

			case FCGI_STDIN:
			/*printf("FCGI_STDIN\t(8+%d B, position %d)\n",
			header.len, position);*/
			fcgi_asemble_body(recived->input, header.len,
			&(buffer[position + FCGI_HEADER_LEN]));
			break;

			//case FCGI_STDOUT:
			//case FCGI_STDERR:
			//case FCGI_DATA:
			//case FCGI_GET_VALUES:
			//case FCGI_GET_VALUES_RESULT:
			//case FCGI_UNKNOWN_TYPE:
			default:
			printf("błond! --\n");
		}

		position += header.len + header.padding + FCGI_HEADER_LEN;
	}

	/* wyświetlenie paramsow */
	int pos = 0, len1, len2;

	while(pos < recived->params->len) 
	{
		fcgi_get_len(&(recived->params->buffer[pos]), &len1, &pos);
		fcgi_get_len(&(recived->params->buffer[pos]), &len2, &pos);

		int j;
		for(j = 0; j < len1; j++) 
		{
			printf("%c", recived->params->buffer[j + pos]);
		}
		printf(": ");

		int k;
		for(k = 0; k < len2; k++) 
		{
			printf("%c", recived->params->buffer[k + j+ pos]);
		}
		pos += k + j;

		printf("\n");
		//printf("pos: %d, k: %d, j: %d\n",pos, k, j);
	}

	printf("\n--- input [%d]---\n", recived->input->len);
	pos = 0;

	while(pos < recived->input->len) 
	{
		if(recived->input->buffer[pos] == '&') printf("\n");
		else printf("%c", recived->input->buffer[pos]);
		pos++;
	}
	printf("\n-------------\n\n");
	pos = 0;

	return 0;
}
