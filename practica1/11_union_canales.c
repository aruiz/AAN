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
	unsigned char *output;
	int i, j, canal_size;
	
	if (!canal1 || !canal2 || !canal_output)
	{
		fprintf (stderr, "Uno de los canales es nulo");
		return;
	}
	if (width < 1 && height < 1)
	{
		fprintf (stderr, "Una de las dimensiones es menor que 1");
		return;
	}

	/* Reservamos la memoria del nuevo canal */
	canal_size = sizeof (unsigned char) * (width*2+4) * height;
	output = *canal_output = (unsigned char*) malloc (canal_size);
	
	for (i = 0; i < width*2 + 4; i++)
	{
		for (j=0; j < height; j++)
		{
			/* Indice del pixel del canal de slida */
			int index = (width*2 + 4) * j + i;
			
			/* Primer canal */
			if (i<width)
				output[index] = canal1[width * j + i];

			/* Pixeles negros */
			else if (i < width+4)
				output[index] = 0;
				
			/* Segundo canal */
			else
				output[index] = canal2[width * j + i - (width+4)];
		}
	}
	
	return;
}

void
aan_unir_canales_float (float  *canal1,
                        float  *canal2,
                        float **canal_output,
                        int   width,
                        int   height)
{
	float *output;
	int i, j, canal_size;
	
	if (!canal1 || !canal2 || !canal_output)
	{
		fprintf (stderr, "Uno de los canales es nulo");
		return;
	}
	if (width < 1 && height < 1)
	{
		fprintf (stderr, "Una de las dimensiones es menor que 1");
		return;
	}

	/* Reservamos la memoria del nuevo canal */
	canal_size = sizeof (float) * (width*2+4) * height;
	output = *canal_output = (float*) malloc (canal_size);
	
	for (i = 0; i < width*2 + 4; i++)
	{
		for (j=0; j < height; j++)
		{
			/* Indice del pixel del canal de slida */
			int index = (width*2 + 4) * j + i;
			
			/* Primer canal */
			if (i<width)
				output[index] = canal1[width * j + i];

			/* Pixeles negros */
			else if (i < width+4)
				output[index] = 0;
				
			/* Segundo canal */
			else
				output[index] = canal2[width * j + i - (width+4)];
		}
	}
	
	return;
}

float*
uchar_to_float (unsigned char *canal, size_t pixeles)
{
	int i;
	float * output = (float*) malloc (sizeof (float) * pixeles);
	for (i=0; i<pixeles; i++)
	{
		/* Pasamos el valor a precision flotante: (0-255) -> (0.0-1.0) */
		output[i] = canal[i] / 255.0;
	}
	return output;
}

unsigned char*
float_to_uchar (float *canal, size_t pixeles)
{
	int i;
	unsigned char * output = (unsigned char*) malloc (sizeof (unsigned char) * pixeles);
	for (i=0; i<pixeles; i++)
	{
		/* Pasamos el valor a precision entera: (0.0-1.0) -> (0-255) */
		output[i] = (int)(canal[i] * 255.0);
	}
	return output;
}

int
main (int argc, char** argv)
{
	int w, h;
	unsigned char *red1, *green1, *blue1,
	              *red2, *green2, *blue2;
	float         *fred1, *fgreen1, *fblue1,
	              *fred2, *fgreen2, *fblue2;
	
	if (argc < 2)
	{
		fprintf (stderr, "Usage: %s <BMP file>\n", argv[0]);
		return -1;
	}

	/* Leemos el fichero dado por el primer argumento */	
	if (ami_read_bmp (argv[1], &red1, &green1, &blue1, &w, &h) < 0)
		return -1;
	
	/********** TEST DE aan_unir_canales_unsigned_char **************/
		
	aan_unir_canales_unsigned_char (red1, red1, &red2, w, h);
	aan_unir_canales_unsigned_char (green1, green1, &green2, w, h);
	aan_unir_canales_unsigned_char (blue1, blue1, &blue2, w, h);
		
	ami_write_bmp ("./11_union_canales_rgb.bmp", red2, green2, blue2, w*2 + 4, h);
	/* Liberamos la memoria usada para esta imagen  */	
	free (red2); free (green2); free (blue2);
	
	/************* TEST DE aan_unir_canales_float ******************/
	/* Transformamos los tres canales en canales flotantes*/	
	fred1   = uchar_to_float (red1,   w * h);
	fgreen1 = uchar_to_float (green1, w * h);
	fblue1  = uchar_to_float (blue1,  w * h);
	
	aan_unir_canales_float (fred1,   fred1,   &fred2,   w, h);
	aan_unir_canales_float (fgreen1, fgreen1, &fgreen2, w, h);
	aan_unir_canales_float (fblue1,  fblue1,  &fblue2,  w, h);
	
	/* Transformamos los tres canales en canales enteros para guardarlos en BMP */
	red2   = float_to_uchar (fred2,   (w*2 + 4) * h);
	green2 = float_to_uchar (fgreen2, (w*2 + 4) * h);
	blue2  = float_to_uchar (fblue2,  (w*2 + 4) * h);
	
	ami_write_bmp ("./11_union_canales_float.bmp", red2, green2, blue2, w*2 + 4, h);

	/* Liberamos la memoria usada */
	free (red1); free (green1); free (blue1);
	free (red2); free (green2); free (blue2);
	free (fred1); free (fgreen1); free (fblue1);
	free (fred2); free (fgreen2); free (fblue2);
	return 0;
}
