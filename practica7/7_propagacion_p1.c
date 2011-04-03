#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ami_bmp.h"
#include "float_utils.h"
#include "aan_propagacion.h"

#define NITER 200
#define DT    0.5

#define K     1.0

float*
calcular_f_p1 (float *v, int width, int height, float k)
{
	int i, j;
	float *f_p1 = (float*)malloc (sizeof(float) * width * height);

	for (i=0; i<height; i++)
	{
		for (j=0; j<width; j++)
		{
			f_p1[i*width + j] = k*(1.0 - )/1.0
		}
	}
	
	return f_p1;
}

int
main (int argc, char** argv)
{
	int w, h;
	unsigned char *red, *green, *blue;
	float         *fred1, *fgreen1, *fblue1;
	
	float *f_p1;

	if (argc < 2)
	{
		fprintf (stderr, "Usage: %s <BMP file>\n", argv[0]);
		return -1;
	}

	/* Leemos el fichero dado por el primer argumento */
	if (ami_read_bmp (argv[1], &red, &green, &blue, &w, &h) < 0)
		return -1;


	fred1 = float_to_uchar (red);
	fred1 = float_to_uchar (green);
	fred1 = float_to_uchar (blue);
	
	
	/* Liberamos memoria */
	free (red); free (green); free (blue);
	
	
	
	
	ami_write_bmp ("imagen_ecualizada_e2.bmp", red, green, blue, w, h);
	
	
	return 0;
}
