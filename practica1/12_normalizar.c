#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ami.h"
#include "ami_bmp.h"

#include "float_utils.h"

#include "aan_unir_canales.h"
#include "aan_normalizar.h"

int
main (int argc, char** argv)
{
	int w, h;
	unsigned char *red1, *green1, *blue1,
	              *red2, *green2, *blue2,
	              *red3, *green3, *blue3;

	float *fred1, *fgreen1, *fblue1;

	if (argc < 2)
	{
		fprintf (stderr, "Usage: %s <BMP file>\n", argv[0]);
		return -1;
	}

	/* Leemos el fichero dado por el primer argumento */	
	if (ami_read_bmp (argv[1], &red1, &green1, &blue1, &w, &h) < 0)
		return -1;
	
	/*************************************************************/	
	/* Comprobamos la implementacion para tres canales de 8 bits */
	/*************************************************************/

	/* Creamos tres nuevos canales para los valores normalizados */	
	red2   = (unsigned char *) malloc (sizeof (unsigned char) * w * h);
	green2 = (unsigned char *) malloc (sizeof (unsigned char) * w * h);
	blue2  = (unsigned char *) malloc (sizeof (unsigned char) * w * h);
	
	/* Copiamos los canales de la imagen */
	memcpy ((void*)red2,   (const void*)red1,   sizeof(unsigned char) * w * h);
	memcpy ((void*)green2, (const void*)green1, sizeof(unsigned char) * w * h);
	memcpy ((void*)blue2,  (const void*)blue1,  sizeof(unsigned char) * w * h);
	
	/* Normalizamos */
	aan_normalizar_imagen_unsigned_char (red2, green2, blue2, w, h);
	
	/* Creamos la imagen comparativa resultante uniendo los canales */
	aan_unir_canales_unsigned_char (red1, red2, &red3, w, h);
	aan_unir_canales_unsigned_char (green1, green2, &green3, w, h);
	aan_unir_canales_unsigned_char (blue1, blue2, &blue3, w, h);
	
	ami_write_bmp ("./12_normalizar_rgb.bmp", red3, green3, blue3, w*2 + 4, h);
	
	/* Liberar canales en desuso */
	free (red2); free (green2); free (blue2);
	free (red3); free (green3); free (blue3);
	
	/********************************************************************/
	/* Comprobamos la implementacion para canales de precision flotante */
	/********************************************************************/
	
	/* Convertimos los canales originales a sus valores en precision flotante */
	fred1 = uchar_to_float (red1, w * h);
	fgreen1 = uchar_to_float (green1, w * h);
	fblue1 = uchar_to_float (blue1, w * h);
	
	/* Normalizamos estos canales */
	aan_normalizar_imagen_float (fred1, fgreen1, fblue1, w, h);
	
	/* Los reconvertimos a precision de 8 bits */
	red2 = float_to_uchar (fred1, w * h);
	green2 = float_to_uchar (fgreen1, w * h);
	blue2 = float_to_uchar (fblue1, w * h);

	/* Creamos la imagen resultante */
	aan_unir_canales_unsigned_char (red1, red2, &red3, w, h);
	aan_unir_canales_unsigned_char (green1, green2, &green3, w, h);
	aan_unir_canales_unsigned_char (blue1, blue2, &blue3, w, h);

	ami_write_bmp ("./12_normalizar_float.bmp", red3, green3, blue3, w*2 + 4, h);

	/* Liberamos toda la memoria usada */
	free (red1); free (green1); free (blue1);
	free (red2); free (green2); free (blue2);
	free (red3); free (green3); free (blue3);
	free (fred1); free (fgreen1); free (fblue1);
	
	return 0;
}
