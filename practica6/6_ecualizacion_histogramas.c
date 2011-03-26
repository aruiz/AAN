#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ami_bmp.h"

#include "aan_ecualizar_histograma.h"
#include "float_utils.h"

float*
generar_histograma (unsigned char* canal, int width, int height)
{
	int i, j;

	float *histograma = (float*)malloc (sizeof(float) * 256);
	
	for (i=0;i<256;i++)
	{
		int valor = 0;
		for (j=0; j<(width *height); j++)
		{
			if (canal[j] == i)
				valor++;
		}

		histograma[i] = (float)valor;
	}
	return histograma;
}

void
normalizar_histograma (float* histograma)
{
	int i;
	float sum = 0.0;
	float factor;
	
	/* Hayamos la suma total */
	for (i=0; i<256; i++)
	{
		sum = sum + histograma[i];
	}

	/* Hayamos el factor resultante para normalizar los valores */
	factor = 1.0/sum;
	
	/* Normalizamos los valores */
	for (i=0; i<256; i++)
	{
		histograma[i] = histograma[i] * factor;
	}
}

int
main (int argc, char** argv)
{
	int i, w, h;
	unsigned char *red, *green, *blue;

	float *hr, *hg, *hb;
	float *e1, *e2;

	char salida[100];

	if (argc < 2)
	{
		fprintf (stderr, "Usage: %s <BMP file>\n", argv[0]);
		return -1;
	}

	/* Leemos el fichero dado por el primer argumento */
	if (ami_read_bmp (argv[1], &red, &green, &blue, &w, &h) < 0)
		return -1;
		
	hr = generar_histograma (red,   w, h);
	hg = generar_histograma (green, w, h);
	hb = generar_histograma (blue,  w, h);
	
	for (i=0;i<256;i++)
	{
		fprintf (stderr, "red - %d - %f \n", i, hr[i]);
	}
	
	/* Liberamos memoria y retornamos */
	free (red); free (green); free (blue);
	return 0;
}
