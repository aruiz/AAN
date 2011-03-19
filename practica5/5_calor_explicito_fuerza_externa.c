#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ami_bmp.h"

#include "aan_calor_explicito_fuerza_externa.h"
#include "float_utils.h"

#define DT     0.1
#define NITERS 200
#define A      1.0

#ifdef P1
#define T0     0.0
#define T1     0.5
#define T2     1.0
#endif

int*
generar_histograma (unsigned char* canal, int width, int height)
{
	int i, j;

	int *histograma = (int*)malloc (sizeof(int) * 256);
	for (i=0;i<256;i++)
	{
		int valor = 0;
		for (j=0;j<(width *height);j++)
		{
			if (canal[j] == i)
				valor++;
		}
		histograma[i] = valor;
	}
	return histograma;
}

void
canal_a_valores_t (unsigned char *canal, int w, int h, float* t0, float *t1, float *t2)
{
	int i, a, b;
	int t0_tmp, t1_tmp, t2_tmp;
	int* histograma = generar_histograma (canal, w, h);
	
	/* Calculamos T1 */
	a = b = 0;
	for (i=0; i<256; i++)
	{
		a = a + i*histograma[i];
		b = b + histograma[i];
	}
	t1_tmp = a/b;
	
	/* Calculamos T0 */
	a = b = 0;
	for (i=0; i<t1_tmp; i++)
	{
		a = a + i*histograma[i];
		b = b + histograma[i];
	}
	t0_tmp = a/b;
	
	/* Calculamos T2 */
	a = b = 0;
	for (i=t1_tmp; i<256; i++)
	{
		a = a + i*histograma[i];
		b = b + histograma[i];
	}
	t2_tmp = a/b;
	
	free (histograma);

	*t0 = t0_tmp / 255.0;
	*t1 = t1_tmp / 255.0;
	*t2 = t2_tmp / 255.0;
}

int
main (int argc, char** argv)
{
	int i, w, h;
	unsigned char *red, *green, *blue;
	float *fred1, *fgreen1, *fblue1,
		    *fred2, *fgreen2, *fblue2;
	char salida[100];
	float t0red,   t1red,   t2red,
	      t0green, t1green, t2green,
	      t0blue,  t1blue,  t2blue;

	if (argc < 2)
	{
		fprintf (stderr, "Usage: %s <BMP file>\n", argv[0]);
		return -1;
	}

	/* Leemos el fichero dado por el primer argumento */
	if (ami_read_bmp (argv[1], &red, &green, &blue, &w, &h) < 0)
		return -1;

#ifdef P1
	t0red = t0green = t0blue = T0;
	t1red = t1green = t1blue = T1;
	t2red = t2green = t2blue = T2;
#endif

#ifdef P2
	canal_a_valores_t (red,   w, h, &t0red,   &t1red,   &t2red);
	canal_a_valores_t (green, w, h, &t0green, &t1green, &t2green);
	canal_a_valores_t (blue,  w, h, &t0blue,  &t1blue,  &t2blue);
#endif

	/* Copiamos de uchar a float */
	fred1   = uchar_to_float (red,   w*h);
	fgreen1 = uchar_to_float (green, w*h);
	fblue1  = uchar_to_float (blue,  w*h);

	/* Liberamos memoria */
	free (red); free (green); free (blue);

	fred2   = (float*)malloc (sizeof(float) * w * h);
	fgreen2 = (float*)malloc (sizeof(float) * w * h);
	fblue2  = (float*)malloc (sizeof(float) * w * h);

	for (i = 0; i < NITERS; i++)
	{
		sprintf(salida, "imagen_1%05d.bmp", i);
		
		fprintf (stderr, "%d/%d - dt=%f - A=%f :: Generando fichero %s\n", i, NITERS, DT, A, salida);
		
		aan_ecuacion_calor_fuerza_externa_metodo_explicito (fred1,   fred2,   w, h, DT, 1, A, t0red,   t1red,   t2red);
		aan_ecuacion_calor_fuerza_externa_metodo_explicito (fgreen1, fgreen2, w, h, DT, 1, A, t0green, t1green, t2green);
		aan_ecuacion_calor_fuerza_externa_metodo_explicito (fblue1,  fblue2,  w, h, DT, 1, A, t0blue,  t1blue,  t2blue);
				
		red   = float_to_uchar (fred2,   w*h);
		green = float_to_uchar (fgreen2, w*h);
		blue  = float_to_uchar (fblue2,  w*h);

		memcpy (fred1,   fred2,   sizeof(float) * w * h);
		memcpy (fgreen1, fgreen2, sizeof(float) * w * h);
		memcpy (fblue1,  fblue2,  sizeof(float) * w * h);

		ami_write_bmp (salida, red, green, blue, w, h);

		free (red); free (green); free (blue);
	}

	free (fred2); free (fgreen2); free (fblue2);
	
	return 0;
}
