#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "ami.h"

/* Esta funcion */
void
aan_ecuacion_calor_metodo_explicito_canal (float  *canal_input,
                                           float  *canal_output,
                                           int   width,
                                           int   height,
                                           float dt)
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
			
			/* Hallamos el resultado y lo guardamos en el canal de salida */
			
			/* Inicializamos el valor resultante con el de la misma coordenada del canal de entrada */
			canal_output[width * j + i] = area[1][1]; /* Tambien canal_input[width * j + i] */
			
			/* Recorremos todos los valores del area afectada*/
			for (k=0; k < 3; k++)
			{
				for (l=0; l < 3; l++)
				{
					/* Hacemos la sumatoria de cada elemento */
					/* Separamos el pixel central de los de al rededor */
					if (l==1 && k==1)
						canal_output[width * j + i] = canal_output[width * j + i] + ((dt/3.0) * (-8.0 * area[k][l]));
					else
						canal_output[width * j + i] = canal_output[width * j + i] + ((dt/3.0) * area[k][l]);
				}
			}
		}
	}

	ami_free2d (area);
}


void
aan_ecuacion_calor_metodo_explicito(float *red_input,
                                    float *green_input,
                                    float *blue_input,
                                    float *red_output,
                                    float *green_output,
                                    float *blue_output,
                                    int width,
                                    int height,
                                    float dt, int Niter)
{
	int i;

	float *red_tmp   = (float*)malloc (sizeof (float) * width * height);
	float *green_tmp = (float*)malloc (sizeof (float) * width * height);
	float *blue_tmp  = (float*)malloc (sizeof (float) * width * height);
	memcpy (red_tmp,   red_input,   sizeof(float) * width * height);
	memcpy (green_tmp, green_input, sizeof(float) * width * height);
	memcpy (blue_tmp,  blue_input,  sizeof(float) * width * height);

	for (i=0; i<Niter; i++)
	{
		aan_ecuacion_calor_metodo_explicito_canal (red_input, red_output, width, height, dt);
		aan_ecuacion_calor_metodo_explicito_canal (green_input, green_output, width, height, dt);
		aan_ecuacion_calor_metodo_explicito_canal (blue_input, blue_output, width, height, dt);
		
		memcpy (red_tmp,   red_output,   sizeof(float) * width * height);
		memcpy (green_tmp, green_output, sizeof(float) * width * height);
		memcpy (blue_tmp,  blue_output,  sizeof(float) * width * height);
	}

	free(red_tmp);
	free(green_tmp);
	free(blue_tmp);
}
