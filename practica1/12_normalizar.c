#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ami.h"
#include "ami_bmp.h"

#include "aan_unir_canales.h"
#include "aan_normalizar.h"

int
main (int argc, char** argv)
{
	int w, h;
	unsigned char *red1, *green1, *blue1,
	              *red2, *green2, *blue2,
	              *red3, *green3, *blue3;

	if (argc < 2)
	{
		fprintf (stderr, "Usage: %s <BMP file>\n", argv[0]);
		return -1;
	}

	/* Leemos el fichero dado por el primer argumento */	
	if (ami_read_bmp (argv[1], &red1, &green1, &blue1, &w, &h) < 0)
		return -1;
	
	/* Comprobamos la implementacion para tres canales de 8 bits */
	red2   = (unsigned char *) malloc (sizeof (unsigned char) * w * h);
	green2 = (unsigned char *) malloc (sizeof (unsigned char) * w * h);
	blue2  = (unsigned char *) malloc (sizeof (unsigned char) * w * h);
	
	memcpy ((void*)red2,   (const void*)red1,   sizeof(unsigned char) * w * h);
	memcpy ((void*)green2, (const void*)green1, sizeof(unsigned char) * w * h);
	memcpy ((void*)blue2,  (const void*)blue1,  sizeof(unsigned char) * w * h);
	
	aan_normalizar_imagen_unsigned_char (red2, green2, blue2, w, h);
	
	aan_unir_canales_unsigned_char (red1, red2, &red3, w, h);
	aan_unir_canales_unsigned_char (green1, green2, &green3, w, h);
	aan_unir_canales_unsigned_char (blue1, blue2, &blue3, w, h);
	
	ami_write_bmp ("./12_normalizar_rgb.bmp", red3, green3, blue3, w*2 + 4, h);

	return 0;
}
