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

float*
generar_e1 ()
{
	int i;
	float *e1 = (float*) malloc (sizeof(float) * 256);
	for (i=0; i<256; i++)
	{
		e1[i] = 1.0/256.0;
	}
	return e1;
}

float*
generar_e2 ()
{
	int i;
	float *e2 = (float*) malloc (sizeof(float) * 256);
	for (i=0; i<256; i++)
	{
		if (i<128)
		{
			/* Hayamos la ecuacion de la recta */
			float m = ((1.0/128.0)-(1.0/512.0)) / (127.0 - 0.0);
			float b = 1.0/512.0;
			
			e2[i] = m * i + b;
		}
		else
		{
			float m = ((1.0/512.0)-(1.0/128.0)) / (255.0 - 128.0);
			float b = - (m * 128.0) + (1.0/128.0);
			
			e2[i] = m * i + b;
		}
	}
	return e2;
}

int
main (int argc, char** argv)
{
	int i, w, h;
	unsigned char *red, *green, *blue;

	float tmp = 0.0;

	float *hr, *hg, *hb;
	float *e1, *e2;

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
	
	normalizar_histograma(hr);
	normalizar_histograma(hg);
	normalizar_histograma(hr);

	e1 = generar_e1();
	e2 = generar_e2();
	
	normalizar_histograma(e1);
	normalizar_histograma(e2);
	
	/* Liberamos memoria y retornamos */
	free (red); free (green); free (blue);
	return 0;
}
