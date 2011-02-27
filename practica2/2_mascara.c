#include <stdlib.h>
#include <math.h>

#include "ami.h"
#include "ami_bmp.h"

#include "float_utils.h"

float
pixel_resultante (float **a, float **b)
{
	int i,j;
	float r = 0.0;
	for (j=0; j < 3; j++)
	{
		for (i=0; i < 3; i++)
		{
			r += a[j][i] * b[j][i];
		}
	}
	return r;
}

void
aan_mascara_canal (float  *canal_input,
                   float  *canal_output,
                     int   width,
                     int   height,
                   float **m)
{
	int i, j,
	    k, l;
	float **area;
	
	/* Matriz para el area afectada por la mascara */
	ami_malloc2d (area, float, 3, 3);

	for (j=1; j < height-1; j++)	
	{
		for (i=1; i < width-1; i++)
		{
			int index = width * j + i;
			/* FIXME: Se esta descartando los pixeles de los bordes del canal */
			
			/* Copiamos el area afectada por la mascara para el pixel (i,j) */
			for (k=0; k < 3; k++)
				for (l=0; l < 3; l++)
					area[k][0] = canal_input[(width * (j - 1 + l)) + (i - 1 + k)];
			
			canal_output[index] = pixel_resultante (m, area);
		}		
	}
}

void
aan_mascara_imagen (float *red_input,
                    float *green_input,
                    float *blue_input,
                    float *red_output,
                    float *green_output,
                    float *blue_output,
                    int width,
                    int height,
                    float **m)
{
	aan_mascara_canal (red_input,   red_output,   width, height, m);
	aan_mascara_canal (green_input, green_output, width, height, m);
	aan_mascara_canal (blue_input,  blue_output,  width, height, m);
}

int
main (int argc, char **argv)
{
	int w, h;
	unsigned char *red1, *green1, *blue1,
	              *red2, *green2, *blue2;
	float         *fred1, *fgreen1, *fblue1,
	              *fred2=0, *fgreen2=0, *fblue2=0;
	              
	              
	
	float **u_x, **u_y, **lap;

	ami_malloc2d (u_x, float, 3, 3);
	ami_malloc2d (u_y, float, 3, 3);
	ami_malloc2d (lap, float, 3, 3);

	/* Gradiente horizontal */
	u_x[0][0] = 0.25 * -(2.0 - sqrt(2.0));     u_x[0][1] = 0; u_x[0][2] = 0.25 * (2.0 - sqrt(2.0));
	u_x[1][0] = 0.25 * -2.0 * (sqrt(2.0) - 1); u_x[1][1] = 0; u_x[1][2] = 0.25 * 2.0 * (sqrt(2.0) - 1) ;
	u_x[2][0] = 0.25 * -(2.0-sqrt(2.0));       u_x[2][1] = 0; u_x[2][2] = 0.25 * (2.0 - sqrt(2.0));
	
	/* Gradiente vertical */
	u_y[0][0] = 0.25 * -(2.0 - sqrt(2.0)); u_y[0][1] = 0.25 * -2.0 * (sqrt(2.0) - 1); u_y[0][2] = 0.25 * -(2.0-sqrt(2.0));
	u_y[1][0] = 0;                         u_y[1][1] = 0;                             u_y[1][2] = 0;
	u_y[2][0] = 0.25 *  (2.0 - sqrt(2.0)); u_y[2][1] = 0.25 *  2.0 * (sqrt(2.0) - 1); u_y[2][2] = 0.25 *  (2.0 - sqrt(2.0));
	
	if (argc < 2)
	{
		fprintf (stderr, "Usage: %s <BMP file>\n", argv[0]);
		return -1;
	}

	/* Leemos el fichero dado por el primer argumento */	
	if (ami_read_bmp (argv[1], &red1, &green1, &blue1, &w, &h) < 0)
		return -1;
	
	/* Pasamos los canales a precision flotante */
	fred1   = uchar_to_float (red1,   w * h);
	fgreen1 = uchar_to_float (green1, w * h);
	fblue1  = uchar_to_float (blue1,  w * h);
	
	fred2   = (float*) malloc (sizeof (float) * w * h);
	fgreen2 = (float*) malloc (sizeof (float) * w * h);
	fblue2  = (float*) malloc (sizeof (float) * w * h);
		
	aan_mascara_imagen (fred1, fgreen1, fblue1,
	                    fred2, fgreen2, fblue2,
	                    w, h, u_x);
	
	red2   = float_to_uchar (fred2,   w * h);
	green2 = float_to_uchar (fgreen2, w * h);
	blue2  = float_to_uchar (fblue2,  w * h);
	
	ami_write_bmp ("./2_gradiente_horizontal.bmp", red2, green2, blue2, w, h);

	aan_mascara_imagen (fred1, fgreen1, fblue1,
	                    fred2, fgreen2, fblue2,
	                    w, h, u_y);
	
	red2   = float_to_uchar (fred2,   w * h);
	green2 = float_to_uchar (fgreen2, w * h);
	blue2  = float_to_uchar (fblue2,  w * h);
	
	ami_write_bmp ("./2_gradiente_vertical.bmp", red2, green2, blue2, w, h);
		
	return 0;
}
