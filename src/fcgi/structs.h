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

struct fcgi_recv  
{
	char *buffer;

	struct fcgi_body *params;
	struct fcgi_body *input;
};

struct fcgi_body 
{
	char *buffer;
	int len;
	char done;
};
