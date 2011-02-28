#include <stdlib.h>
#include <math.h>

#include "ami.h"
#include "ami_bmp.h"

#include "aan_normalizar.h"
#include "float_utils.h"

float
pixel_resultante (float **a, float **b)
{
	int i,j;
	float r = 0.0;
	
	/* Sumamos los productos de cada uno de los elementos de cada matriz */
	for (j=0; j < 3; j++)
	{
		for (i=0; i < 3; i++)
		{
			r = r + (a[j][i] * b[j][i]);
		}
	}
	
	
	if (r < 0.0)
		r = 0.0;
	if (r > 1.0)
		r = 1.0;
	
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

	/* Hallamos el area afectada, utilizamos el pixel valido mas cercano en el caso
	 * de estar en uno de los bordes o esquinas.
	 */	
	for (j=0; j < height; j++)
	{
		for (i=0; i < width; i++)
		{
			/*Esquina superior izquierda */
			if (i==0 && j==0)
			{
				area[0][0] = canal_input[0];
				area[0][1] = canal_input[0];
				area[0][2] = canal_input[1];
				area[1][0] = canal_input[0];
				area[2][0] = canal_input[width];

				for (k=1; k < 3; k++)
					for (l=1; l < 3; l++)
						area[k][l] = canal_input[(width * (j - 1 + k)) + (i - 1 + l)];
					
			}
			/* Esquina superior derecha */
			else if (i == (width - 1) && j == 0)
			{
				area[0][2] = canal_input[width-1];
				area[0][1] = canal_input[width-1];
				area[0][0] = canal_input[width-2];
				area[1][2] = canal_input[width-1];
				area[2][2] = canal_input[width*2-1];
				
				for (k=1; k < 3; k++)
					for (l=0; l < 2; l++)
						area[k][l] = canal_input[(width * (j - 1 + k)) + (i - 1 + l)];
			}
			/* Esquina inferior izquierda */
			else if (i == 0 && j == (height - 1))
			{
				area[0][2] = canal_input[width * (j-1)];
				area[0][1] = canal_input[width * (j-1)];
				area[1][2] = canal_input[width * (j-1)];
				area[0][0] = canal_input[width * (j-2)];
				area[2][2] = canal_input[(width * (j-1)) + 1];
				
				for (k=0; k < 2; k++)
					for (l=1; l < 3; l++)
						area[k][l] = canal_input[(width * (j - 1 + k)) + (i - 1 + l)];
				
			}
			/* Esquina inferior derecha */
			else if (i == (width - 1) && j == (height - 1))
			{
				area[2][2] = canal_input[width * height - 1];
				area[1][2] = canal_input[width * height - 1];
				area[0][2] = canal_input[width * (height-1) - 1];
				area[2][1] = canal_input[width * height - 1];
				area[2][0] = canal_input[width * height - 2];
				
				for (k=0; k < 2; k++)
					for (l=0; l < 2; l++)
						area[k][l] = canal_input[(width * (j - 1 + k)) + (i - 1 + l)];
			}
			/* Borde izquierdo (fuera de las esquinas) */
			else if (i == 0)
			{
				area[0][0] = canal_input[(width * (j - 1 + k)) + i];
				area[1][0] = canal_input[(width * (j - 1 + k)) + i];
				area[2][0] = canal_input[(width * (j - 1 + k)) + i];
				
				for (k=0; k < 3; k++)
					for (l=1; l < 3; l++)
						area[k][l] = canal_input[(width * (j - 1 + k)) + (i - 1 + l)];
				
			}
			/* Borde derecho (fuera de las esquinas) */
			else if (i == (width-1))
			{
				area[0][2] = canal_input[(width * (j - 1 + k)) + i];
				area[1][2] = canal_input[(width * (j - 1 + k)) + i];
				area[2][2] = canal_input[(width * (j - 1 + k)) + i];
				
				for (k=0; k < 3; k++)
					for (l=0; l < 2; l++)
						area[k][l] = canal_input[(width * (j - 1 + k)) + (i - 1 + l)];
			}
			/* Borde superior (fuera de las esquinas) */
			else if (j == 0)
			{
				area[0][0] = canal_input[(width * j) + (i - 1 + l)];
				area[0][1] = canal_input[(width * j) + (i - 1 + l)];
				area[0][2] = canal_input[(width * j) + (i - 1 + l)];
				
				for (k=1; k < 3; k++)
					for (l=0; l < 3; l++)
						area[k][l] = canal_input[(width * (j - 1 + k)) + (i - 1 + l)];
			}
			/* Borde inferior (fuera de las esquinas) */
			else if (j == (height-1))
			{
				area[2][0] = canal_input[(width * j) + (i - 1 + l)];
				area[2][1] = canal_input[(width * j) + (i - 1 + l)];
				area[2][2] = canal_input[(width * j) + (i - 1 + l)];
				
				for (k=0; k < 2; k++)
					for (l=0; l < 3; l++)
						area[k][l] = canal_input[(width * (j - 1 + k)) + (i - 1 + l)];
			}
			else
			{
				for (k=0; k < 3; k++)
					for (l=0; l < 3; l++)
						area[k][l] = canal_input[width * (j - 1 + k) + (i - 1 + l)];
			}
			
			/* Hallamos el resultado y lo guardamos en el canal de salida */
			canal_output[width * j + i] = pixel_resultante (m, area);
		}
	}
	
	
	ami_free2d (area);
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
	
	lap[0][0] = 1.0/3.0; lap[0][1] =  1.0/3.0; lap[0][2] = 1.0/3.0;
	lap[1][0] = 1.0/3.0; lap[1][1] = -8.0/3.0; lap[1][2] = 1.0/3.0;
	lap[2][0] = 1.0/3.0; lap[2][1] =  1.0/3.0; lap[2][2] = 1.0/3.0;
	
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
	
	aan_normalizar_imagen_float (fred2, fgreen2, fblue2, w, h);
	
	red2   = float_to_uchar (fred2,   w * h);
	green2 = float_to_uchar (fgreen2, w * h);
	blue2  = float_to_uchar (fblue2,  w * h);
	
/*	ami_write_bmp ("./2_gradiente_horizontal.bmp", red2, green2, blue2, w, h);
	free (red2); free (green2); free (blue2);

	aan_mascara_imagen (fred1, fgreen1, fblue1,
	                    fred2, fgreen2, fblue2,
	                    w, h, u_y);

	aan_normalizar_imagen_float (fred2, fgreen2, fblue2, w, h);
	
	red2   = float_to_uchar (fred2,   w * h);
	green2 = float_to_uchar (fgreen2, w * h);
	blue2  = float_to_uchar (fblue2,  w * h);
	
	ami_write_bmp ("./2_gradiente_vertical.bmp", red2, green2, blue2, w, h);
	free (red2); free (green2); free (blue2);
	
	aan_mascara_imagen (fred1, fgreen1, fblue1,
	                    fred2, fgreen2, fblue2,
	                    w, h, lap);

	aan_normalizar_imagen_float (fred2, fgreen2, fblue2, w, h);
	
	red2   = float_to_uchar (fred2,   w * h);
	green2 = float_to_uchar (fgreen2, w * h);
	blue2  = float_to_uchar (fblue2,  w * h);
	
	ami_write_bmp ("./2_laplace.bmp", red2, green2, blue2, w, h);*/
		
	return 0;
}
