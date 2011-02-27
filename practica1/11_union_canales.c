#include <stdlib.h>

#include "ami.h"
#include "ami_bmp.h"

void
aan_unir_canales_unsigned_char (unsigned char  *canal1,
                                unsigned char  *canal2,
                                unsigned char **canal_output,
                                          int   width,
                                          int   height)
{
	int canal_size;
	
	if (!canal1 || !canal2 || !canal_output)
		return;
	if (width < 1 && height < 1)
		return;

	/* Reservamos la memoria del nuevo canal */
	canal_size = sizeof (unsigned char) * (width*2+4) * height;
	*canal_output = (unsigned char*) malloc (canal_size);
	
	
	
	return;
}

int
main (int argc, char** argv)
{
	return 0;
}
