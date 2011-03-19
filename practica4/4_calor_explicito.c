#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ami_bmp.h"

#include "aan_calor_explicito.h"
#include "float_utils.h"

/* Con los siguie*/
#ifdef P41
#define NITERS 100
#define DT 1.0/4.0
#endif
#ifdef P42
#define NITERS 200
#define DT 1.0/8.0
#endif
#ifdef P43
#define NITERS 300
#define DT 1.0/16.0
#endif
#ifdef P44
#define NITERS 800
#define DT 1.0/2.0
#endif
#ifdef NEGATIVO
#define NITERS 100
#define DT -0.01
#endif

int
main (int argc, char** argv)
{
	int i, w, h;
	unsigned char *red, *green, *blue;
	float *fred1, *fgreen1, *fblue1,
		    *fred2, *fgreen2, *fblue2;
	char salida[100];

	if (argc < 2)
	{
		fprintf (stderr, "Usage: %s <BMP file>\n", argv[0]);
		return -1;
	}

	/* Leemos el fichero dado por el primer argumento */
	if (ami_read_bmp (argv[1], &red, &green, &blue, &w, &h) < 0)
		return -1;

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
		
		fprintf (stderr, "0/%d - dt=%f - Generando fichero %s\n", i, DT, salida);
		
		aan_ecuacion_calor_metodo_explicito(fred1, fgreen1, fblue1,
		                                    fred2, fgreen2, fblue2,
		                                    w, h,
		                                    DT, 1);

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
