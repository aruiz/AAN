#include <stdlib.h>
#include <math.h>

#include "ami.h"
#include "ami_bmp.h"

#include "float_utils.h"
#include "aan_normalizar.h"
#include "aan_unir_canales.h"

#include "aan_mascara.h"

void
combinar (float  *canal1,
          float  *canal2,
          float **canal_output,
            int   width,
            int   height)
{
	int j;
	float *out;
	
	out = *canal_output = (float*)malloc (sizeof(float) * width * height);
	
	for (j=0; j < width * height; j++)
	{
			out[j] = sqrt(pow (canal2[j], 2.0) + pow (canal1[j], 2.0));

			if (out[j] > 1.0)
				out[j] = 1.0;
			if (out[j] < 0.0)
				out[j] = 0.0;				
	}
	
	return;
}


int
main (int argc, char **argv)
{
	int i, w, h;
	unsigned char *red1, *green1, *blue1,
	              *red2, *green2, *blue2,
	              *red3, *green3, *blue3;
	              
	float         *fred1, *fgreen1, *fblue1,
	              *fred2, *fgreen2, *fblue2;
	
	float **u_x, **u_y, **lap;

	if (argc < 2)
	{
		fprintf (stderr, "Usage: %s <BMP file>\n", argv[0]);
		return -1;
	}
	/* Leemos el fichero dado por el primer argumento */	
	if (ami_read_bmp (argv[1], &red1, &green1, &blue1, &w, &h) < 0)
		return -1;
	
	ami_malloc2d (u_x, float, 3, 3);
	ami_malloc2d (u_y, float, 3, 3);
	ami_malloc2d (lap, float, 3, 3);

	/* Gradiente horizontal */
	u_x[0][0] = 0.25 * -(2.0 - sqrt(2.0));     u_x[0][1] = 0; u_x[0][2] = 0.25 * (2.0 - sqrt(2.0));
	u_x[1][0] = 0.25 * -2.0 * (sqrt(2.0) - 1); u_x[1][1] = 0; u_x[1][2] = 0.25 * 2.0 * (sqrt(2.0) - 1);
	u_x[2][0] = 0.25 * -(2.0 - sqrt(2.0));     u_x[2][1] = 0; u_x[2][2] = 0.25 * (2.0 - sqrt(2.0));
	
	/* Gradiente vertical */
	u_y[0][0] = 0.25 * -(2.0 - sqrt(2.0)); u_y[0][1] = 0.25 * -2.0 * (sqrt(2.0) - 1); u_y[0][2] = 0.25 * -(2.0-sqrt(2.0));
	u_y[1][0] = 0;                         u_y[1][1] = 0;                             u_y[1][2] = 0;
	u_y[2][0] = 0.25 *  (2.0 - sqrt(2.0)); u_y[2][1] = 0.25 *  2.0 * (sqrt(2.0) - 1); u_y[2][2] = 0.25 *  (2.0 - sqrt(2.0));
	
	/* Pasamos los canales a precision flotante */
	fred1   = uchar_to_float (red1,   w * h);
	fgreen1 = uchar_to_float (green1, w * h);
	fblue1  = uchar_to_float (blue1,  w * h);

	aan_normalizar_imagen_float (fred1, fgreen1, fblue1, w , h);
	
	fred2   = (float*) malloc (sizeof (float) * w * h);
	fgreen2 = (float*) malloc (sizeof (float) * w * h);
	fblue2  = (float*) malloc (sizeof (float) * w * h);
	
	/* Usamos el gradiente horizontal en el canal verde */
	aan_mascara_canal (fgreen1, fgreen2, w, h, u_x);
	aan_mascara_canal (fred1, fred2, w, h, u_y);
	for (i=0; i < (w * h) ;i++)
		fblue2[i] = 127.0;

	red2   = float_to_uchar (fred2,   w * h);
	green2 = float_to_uchar (fgreen2, w * h);
	blue2  = float_to_uchar (fblue2,  w * h);
	
	aan_normalizar_imagen_float (fred2, fgreen2, fblue2, w , h);
	
	aan_unir_canales_unsigned_char (red1,   red2,   &red3,   w, h);
	aan_unir_canales_unsigned_char (green1, green2, &green3, w, h);
	aan_unir_canales_unsigned_char (blue1,  blue2,  &blue3,  w, h);
	
	ami_write_bmp ("./2_mascara_byn.bmp", red3, green3, blue3, w*2 + 4, h);

	/* Liberamos memoria */
	free (red2); free (green2); free (blue2);
	free (red3); free (green3); free (blue3);

	free (red1); free (green1); free (blue1);
	free (fred1); free (fgreen1); free (fblue1);
	free (fred2); free (fgreen2); free (fblue2);
	ami_free2d (u_x); ami_free2d (u_y);
	return 0;
}
