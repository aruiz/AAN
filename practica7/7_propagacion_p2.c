#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "ami.h"
#include "ami_bmp.h"
#include "float_utils.h"
#include "aan_propagacion.h"

#define NITER 200
#define DT    0.8
#define K     0.9
#define M     0.5

void* gradiente_horizontal (float *input, int width, int height);
void* gradiente_vertical (float *input, int width, int height);
void* modgradiente (float *input, int width, int height);

float*
calcular_f_p2 (float *v, int width, int height, float k, float m)
{
	int i, j;
	float *f_p2 = (float*)malloc (sizeof(float) * width * height);
	
	float *grad = modgradiente (v, width, height);

	for (i=0; i<height; i++)
	{
		for (j=0; j<width; j++)
		{
			float tmp = grad[i*width + j] / m;
			
			if (tmp > 1.0)
				tmp = 1.0;
			
			f_p2[i*width + j] = k * (1.0 - tmp);
		}
	}

	return f_p2;
}

int
main (int argc, char** argv)
{
	int i, w, h;
	unsigned char *red, *green, *blue;
	float         *fred1, *fgreen1, *fblue1,
	              *fred2, *fgreen2, *fblue2;
	
	float *f_p2;

	if (argc < 2)
	{
		fprintf (stderr, "Usage: %s <BMP file>\n", argv[0]);
		return -1;
	}

	/* Leemos el fichero dado por el primer argumento */
	if (ami_read_bmp (argv[1], &red, &green, &blue, &w, &h) < 0)
		return -1;


	fred1   = uchar_to_float (red,   w * h);
	fgreen1 = uchar_to_float (green, w * h);
	fblue1  = uchar_to_float (blue,  w * h);
	/* Liberamos memoria */
	free (red); free (green); free (blue);
	
	f_p2 = calcular_f_p2 (fgreen1, w, h, K, M);
	
	fred2 = (float*)malloc (sizeof(float) * w * h);
	fgreen2 = (float*)malloc (sizeof(float) * w * h);
	fblue2 = (float*)malloc (sizeof(float) * w * h);
	
	for (i=0; i<NITER; i++)
	{
		char salida[100];
		sprintf(salida, "imagen_1%05d.bmp", i);
		fprintf (stderr, "%d/%d - Generando fichero %s\n", i, NITER, salida);

		aan_ecuacion_propagacion_frentes(fred1, fgreen1, fblue1,
		                                 fred2, fgreen2, fblue2,
		                                 w, h, DT, 1, f_p2);

		red =   float_to_uchar (fred2,   w*h);
		green = float_to_uchar (fgreen2, w*h);
		blue =  float_to_uchar (fblue2,  w*h);

		ami_write_bmp (salida, red, green, blue, w, h);

		memcpy (fred1,   fred2,   sizeof(float) * w * h);
		memcpy (fgreen1, fgreen2, sizeof(float) * w * h);
		memcpy (fblue1,  fblue2,  sizeof(float) * w * h);

		free (red); free (green); free (blue);
	}

	free (fred1); free (fgreen1); free (fblue1);
	free (fred2); free (fgreen2); free (fblue2);

	return 0;
}
