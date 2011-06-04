#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "float_utils.h"
#include "aan_mascara.h"
#include "ami.h"

float*
canal_a_k (float *input, int width, int height, float lambda)
{
	int i;
	float **lap;
	float *output = (float*) malloc (sizeof(float) * width * height);
	ami_malloc2d (lap, float, 3, 3);

	/* Gradiente laplaciano */
	lap[0][0] = 1.0/3.0; lap[0][1] =  1.0/3.0; lap[0][2] = 1.0/3.0;
	lap[1][0] = 1.0/3.0; lap[1][1] = -8.0/3.0; lap[1][2] = 1.0/3.0;
	lap[2][0] = 1.0/3.0; lap[2][1] =  1.0/3.0; lap[2][2] = 1.0/3.0;

	aan_mascara_canal (input, output, width, height, lap);

	for (i=0; i < width * height; i++)
	{
		output[i] = powf (M_E, output[i] * -lambda);

		if (output[i] < 0.0)
			output[i] = 0.0;
		else if (output[i] > 1.0)
			output[i] = 1.0;
	}

	ami_free2d (lap);
	return output;
}

float**
hallar_gradiente (float *canal_input, int width, int height, int i, int j)
{
	int k, l;
	float **area;
	float **grad;

	ami_malloc2d (grad, float, 3, 3);
	ami_malloc2d (area, float, 3, 3);

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
		area[0][0] = canal_input[(width * (j - 1 + 0)) + i];
		area[1][0] = canal_input[(width * (j - 1 + 1)) + i];
		area[2][0] = canal_input[(width * (j - 1 + 2)) + i];
		
		for (k=0; k < 3; k++)
			for (l=1; l < 3; l++)
				area[k][l] = canal_input[(width * (j - 1 + k)) + (i - 1 + l)];
		
	}
	/* Borde derecho (fuera de las esquinas) */
	else if (i == (width-1))
	{
		area[0][2] = canal_input[(width * (j - 1 + 0)) + i];
		area[1][2] = canal_input[(width * (j - 1 + 1)) + i];
		area[2][2] = canal_input[(width * (j - 1 + 2)) + i];
		
		for (k=0; k < 3; k++)
			for (l=0; l < 2; l++)
				area[k][l] = canal_input[(width * (j - 1 + k)) + (i - 1 + l)];
	}
	/* Borde superior (fuera de las esquinas) */
	else if (j == 0)
	{
		area[0][0] = canal_input[(width * j) + (i - 1 + 0)];
		area[0][1] = canal_input[(width * j) + (i - 1 + 1)];
		area[0][2] = canal_input[(width * j) + (i - 1 + 2)];
		
		for (k=1; k < 3; k++)
			for (l=0; l < 3; l++)
				area[k][l] = canal_input[(width * (j - 1 + k)) + (i - 1 + l)];
	}
	/* Borde inferior (fuera de las esquinas) */
	else if (j == (height-1))
	{
		area[2][0] = canal_input[(width * j) + (i - 1 + 0)];
		area[2][1] = canal_input[(width * j) + (i - 1 + 1)];
		area[2][2] = canal_input[(width * j) + (i - 1 + 2)];
		
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

	
	grad[0][0] = 0.0;                     grad[0][1] = area[1][1] + area[2][1]; grad[0][2] = 0.0;
	grad[1][0] = area[1][1] + area[1][0];                                       grad[1][2] = area[1][1] + area[1][2];
	grad[2][0] = 0.0;                     grad[2][1] = area[1][1] + area[0][1]; grad[2][2] = 0.0;
	/* Hallamos el valor central del gradiente */
        grad[1][1] = -area[1][2] -area[1][0] -4*area[1][1] -area[2][1] -area[0][1];

	ami_free2d (area);
	return grad;
}

float*
canal_a_m (float *canal_input, int width, int height, float lambda)
{
	int i,j,k,l;
	float **area;
	float *canal_k = canal_a_k (canal_input, width, height, lambda);

	float *output = (float*) malloc (sizeof (float) * width * height);

	/* Matriz para el area afectada por la mascara */
	ami_malloc2d (area, float, 3, 3);

	/* Hallamos el area afectada, utilizamos el pixel valido mas cercano en el caso
	 * de estar en uno de los bordes o esquinas.
	 */	
	for (j=0; j < height; j++)
	{
		for (i=0; i < width; i++)
		{
			float **grad;
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
				area[0][0] = canal_input[(width * (j - 1 + 0)) + i];
				area[1][0] = canal_input[(width * (j - 1 + 1)) + i];
				area[2][0] = canal_input[(width * (j - 1 + 2)) + i];
				
				for (k=0; k < 3; k++)
					for (l=1; l < 3; l++)
						area[k][l] = canal_input[(width * (j - 1 + k)) + (i - 1 + l)];
				
			}
			/* Borde derecho (fuera de las esquinas) */
			else if (i == (width-1))
			{
				area[0][2] = canal_input[(width * (j - 1 + 0)) + i];
				area[1][2] = canal_input[(width * (j - 1 + 1)) + i];
				area[2][2] = canal_input[(width * (j - 1 + 2)) + i];
				
				for (k=0; k < 3; k++)
					for (l=0; l < 2; l++)
						area[k][l] = canal_input[(width * (j - 1 + k)) + (i - 1 + l)];
			}
			/* Borde superior (fuera de las esquinas) */
			else if (j == 0)
			{
				area[0][0] = canal_input[(width * j) + (i - 1 + 0)];
				area[0][1] = canal_input[(width * j) + (i - 1 + 1)];
				area[0][2] = canal_input[(width * j) + (i - 1 + 2)];
				
				for (k=1; k < 3; k++)
					for (l=0; l < 3; l++)
						area[k][l] = canal_input[(width * (j - 1 + k)) + (i - 1 + l)];
			}
			/* Borde inferior (fuera de las esquinas) */
			else if (j == (height-1))
			{
				area[2][0] = canal_input[(width * j) + (i - 1 + 0)];
				area[2][1] = canal_input[(width * j) + (i - 1 + 1)];
				area[2][2] = canal_input[(width * j) + (i - 1 + 2)];
				
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
			
			output[width * j + i] = 0.0;

			grad = hallar_gradiente (canal_k, width, height, i, j);
	
			/* Aplicamos la mascara a este pixel y guardamos el resultado en la salida */
			for (j=0; j < 3; j++)
				for (i=0; i < 3; i++)
					output[width + j + i] = output[width + j + i] + area[j][i] * grad[j][i];

			ami_free2d (grad);
		}
	}
	
	
	ami_free2d (area);
	free (canal_k);

	return output;
}

void
aan_perona_malik (float *red_input,
                  float *green_input,
                  float *blue_input,
                  float *red_output,
                  float *green_output,
                  float *blue_output,
                  int    width,
                  int    height,
                  float  dt,
                  float  dh,
                  float  lambda,
                  int    Niters)
{
	int i, j, iter;

	float *red_m   = canal_a_m (red_input,   width, height, lambda);
	float *green_m = canal_a_m (green_input, width, height, lambda);
	float *blue_m  = canal_a_m (blue_input,  width, height, lambda);

	float *red   = (float*) malloc (sizeof (float) * width * height);
	float *green = (float*) malloc (sizeof (float) * width * height);
	float *blue  = (float*) malloc (sizeof (float) * width * height);

	memcpy (red,   red_input,   sizeof (float) *  width * height);
	memcpy (green, green_input, sizeof (float) * width * height);
	memcpy (blue,  blue_input,  sizeof (float) * width * height);

	for (iter = 0; iter < Niters; iter++)
	{
		unsigned char *ured, *ugreen, *ublue;

		char imagen[100];
		for (i = 0; i < width; i++)
			for (j = 0; j < height; j++)
			{
				int idx = width * j + i;

				red_output [idx]   = red[idx]   + (dt * red_m[idx])  /dh*dh*2;
				green_output [idx] = green[idx] + (dt * green_m[idx])/dh*dh*2;
				blue_output [idx]  = blue[idx]  + (dt * blue_m[idx]) /dh*dh*2;

				if (red_output[idx] < 0.0)
					red_output[idx] = 0.0;
				else if (red_output[idx] > 1.0)
					red_output[idx] = 1.0;
				if (green_output[idx] < 0.0)
					green_output[idx] = 0.0;
				else if (green_output[idx] > 1.0)
					green_output[idx] = 1.0;
				if (blue_output[idx] < 0.0)
					blue_output[idx] = 0.0;
				else if (blue_output[idx] > 1.0)
					blue_output[idx] = 1.0;
			}
		/* Guardamos el canal en un fichero */
		sprintf(imagen, "imagen_1%05d.bmp", iter);
		ured   = float_to_uchar (red_output,   width * height);
		ugreen = float_to_uchar (green_output, width * height);
		ublue  = float_to_uchar (blue_output,  width * height);

		ami_write_bmp (imagen, ured, ugreen, ublue, width, height);

		free (ured); free (ugreen); free (ublue);
		memcpy (red,   red_output,   sizeof (float) * width * height);
		memcpy (green, green_output, sizeof (float) * width * height);
		memcpy (blue,  blue_output,  sizeof (float) * width * height);

	}
}
