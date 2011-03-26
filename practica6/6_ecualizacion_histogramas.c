#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ami_bmp.h"

#include "aan_ecualizar_histograma.h"
#include "aan_unir_canales.h"
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
	int w, h;
	unsigned char *red, *green, *blue;

	float *fred1, *fgreen1, *fblue1,
          *fred2, *fgreen2, *fblue2,
          *fred3, *fgreen3, *fblue3;
	
	float *hr, *hg, *hb;
	float *e1, *e2;
	int *f;

	if (argc < 2)
	{
		fprintf (stderr, "Usage: %s <BMP file>\n", argv[0]);
		return -1;
	}

	/* Leemos el fichero dado por el primer argumento */
	if (ami_read_bmp (argv[1], &red, &green, &blue, &w, &h) < 0)
		return -1;

	/* Generarmos histogramas de origen */		
	hr = generar_histograma (red,   w, h);
	hg = generar_histograma (green, w, h);
	hb = generar_histograma (blue,  w, h);
	
	/* Generamos histogramas objetivo */
	e1 = generar_e1();
	e2 = generar_e2();
	
	/* Normalizamos los histogramas */
	normalizar_histograma(hr);
	normalizar_histograma(hg);
	normalizar_histograma(hb);
	normalizar_histograma(e1);
	normalizar_histograma(e2);

	fred1   = uchar_to_float (red,   w*h);
	fgreen1 = uchar_to_float (green, w*h);
	fblue1  = uchar_to_float (blue,  w*h);
	free (red); free (green); free (blue);

	/* Reservamos memoria para los canales de salida y el vector de ecualizacion f */
	fred2    = (float*)malloc (sizeof(float) * w*h);
	fgreen2  = (float*)malloc (sizeof(float) * w*h);
	fblue2   = (float*)malloc (sizeof(float) * w*h);

	f = (int*)malloc (sizeof(int) * 256);
	
	/************* Ecualizamos al histograma E1 (y = 1/256) *************/
	aan_ecualizar_histograma(hr, e1, f); /* rojo */
	aan_ecualizar_histograma_canal(fred1,   fred2,   w, h, f);
	aan_ecualizar_histograma(hg, e1, f); /* verde */
	aan_ecualizar_histograma_canal(fgreen1, fgreen2, w, h, f);
	aan_ecualizar_histograma(hb, e1, f); /* azul */
	aan_ecualizar_histograma_canal(fblue1,  fblue2,  w, h, f);
		
	aan_unir_canales_float (fred1,   fred2,   &fred3,   w, h);
	aan_unir_canales_float (fgreen1, fgreen2, &fgreen3, w, h);
	aan_unir_canales_float (fblue1,  fblue2,  &fblue3,  w, h);

	red   = float_to_uchar (fred3,   (w*2 + 4)*h);
	green = float_to_uchar (fgreen3, (w*2 + 4)*h);
	blue  = float_to_uchar (fblue3,  (w*2 + 4)*h);

	ami_write_bmp ("imagen_ecualizada_e1.bmp", red, green, blue, w*2 + 4, h);

	/* Liberamos memoria */
	free (red); free (green); free (blue);
	free (fred3); free (fgreen3); free (fblue3);

	/************* Ecualizamos al histograma E1 (y = 1/256) *************/
	aan_ecualizar_histograma(hr, e2, f); /* rojo */
	aan_ecualizar_histograma_canal(fred1,   fred2,   w, h, f);
	aan_ecualizar_histograma(hg, e2, f); /* verde */
	aan_ecualizar_histograma_canal(fgreen1, fgreen2, w, h, f);
	aan_ecualizar_histograma(hb, e2, f); /* azul */
	aan_ecualizar_histograma_canal(fblue1,  fblue2,  w, h, f);
		
	aan_unir_canales_float (fred1,   fred2,   &fred3,   w, h);
	aan_unir_canales_float (fgreen1, fgreen2, &fgreen3, w, h);
	aan_unir_canales_float (fblue1,  fblue2,  &fblue3,  w, h);

	red   = float_to_uchar (fred3,   (w*2 + 4)*h);
	green = float_to_uchar (fgreen3, (w*2 + 4)*h);
	blue  = float_to_uchar (fblue3,  (w*2 + 4)*h);

	ami_write_bmp ("imagen_ecualizada_e2.bmp", red, green, blue, w*2 + 4, h);
		
	/* Liberamos memoria */
	free (red); free (green); free (blue);
	free (fred1); free (fgreen1); free (fblue1);
	free (fred2); free (fgreen2); free (fblue2);
	free (fred3); free (fgreen3); free (fblue3);
	free (hr); free (hg); free (hb); free(e1); free(e2); free(f);
	return 0;
}
