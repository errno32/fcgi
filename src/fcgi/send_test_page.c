#include <string.h>

int fcgi_send_test_page(int sfd) 
{
	printf("funkcja fcgi_send_test_page()... ");

	unsigned char h1[8];
	h1[0] = 1;
	h1[1] = 6; //type STDOUT
	h1[2] = 0;
	h1[3] = 1;
	h1[4] = 0;
	h1[5] = 115;
	h1[6] = 0;
	h1[7] = 0;

	unsigned char content[] =
		"Content-Type: text/html\n" //24
		"Content-Length: 69" //18
		"\r\n\r\n" //4
		"<html><title>Tytul</title>" //26
		"<body>01234567__abcdefghi</body></html>\r\n\r\n"; //43

		//lacznie: 115

	unsigned char h2[8];
	h2[0] = 1;
	h2[1] = 6; //type STDOUT
	h2[2] = 0;
	h2[3] = 1;
	h2[4] = 0;
	h2[5] = 0;
	h2[6] = 0;
	h2[7] = 0;

	unsigned char h3[16];
	h3[0] = 1;
	h3[1] = 3; //type END_REQUEST
	h3[2] = 0;
	h3[3] = 1;
	h3[4] = 0;
	h3[5] = 8;
	h3[6] = 0;
	h3[7] = 0;

	h3[8] = 0;
	h3[9] = 0;
	h3[10] = 0;
	h3[11] = 1;
	h3[12] = 0; //protocol status = request complete
	h3[13] = 0;
	h3[14] = 0;
	h3[15] = 0;

	char buffer[4096] =  { '\0' };

	memcpy(&buffer[0], h1, 8);
	memcpy(&buffer[8], content, 115);
	memcpy(&buffer[123], h2, 8);
	memcpy(&buffer[131], h3, 16);

	send(sfd, buffer, 147, 0);

	// close(sfd);	/* przenioslem do accept() */
/*
	send(sfd, h1, 8, 0);
	send(sfd, content, 115, 0);
	send(sfd, h2, 8, 0);
	send(sfd, h3, 16, 0);
*/
	printf("-- wyslalem\n");

	return 0;
}
