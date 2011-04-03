#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ami_bmp.h"

int
main (int argc, char** argv)
{
	int w, h;
	unsigned char *red, *green, *blue;

	if (argc < 2)
	{
		fprintf (stderr, "Usage: %s <BMP file>\n", argv[0]);
		return -1;
	}

	/* Leemos el fichero dado por el primer argumento */
	if (ami_read_bmp (argv[1], &red, &green, &blue, &w, &h) < 0)
		return -1;


/*	ami_write_bmp ("imagen_ecualizada_e2.bmp", red, green, blue, w, h); */
		
	/* Liberamos memoria */
	free (red); free (green); free (blue);
	return 0;
}
