#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "conf.h"

#include "src/rec/record.h"


int main(void)
{
	record rec;
	REC("Rozpoczęcie pracy programu.%s", "");

	REC("Wszystko oki, panie %s!", "Janie");
	REC_ERR(OK, 32, "Złamana rura, hihi %s", ":-)");
	REC_ERR(1, 2, "ojej, to je %d numer", 12);

	REC("Zakończenie pracy programu.%s", "");
	return 0;
}	


