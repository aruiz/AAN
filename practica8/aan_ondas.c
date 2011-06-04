#include <stdlib.h>
#include <string.h>

#include "aan_ondas.h"
#include "float_utils.h"


#include "ami_bmp.h"
#include "ami.h"

void
aan_ondas_un_canal (float *canal_input,
                    float *canal_output,
                    float *canal_anterior,
                    int    width,
                    int    height,
                    float  dt)
{
  int i, j,
	    k, l;
  float   tmp;
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

      tmp = 0;

			/* Recorremos todos los valores del area afectada*/
			for (k=0; k < 3; k++)
			{
				for (l=0; l < 3; l++)
				{
            if (k == 1 && l == 1)
  						tmp = tmp + (-8.0 * area[1][1]);
            else
              tmp = tmp + area[k][l];
				}
			}
			
      canal_output[width * j + i] = 2.0 * area[1][1] - canal_anterior[width * j + i] + dt*dt*tmp;

      if (canal_output[width * j + i] > 1.0)
        canal_output[width * j + i] = 1.0;
      else if (canal_output[width * j + i] < 0.0)
        canal_output[width * j + i] = 0.0;
		}
	}

	ami_free2d (area); 
}

void
aan_ecuacion_ondas_metodo_explicito(float *red_input,
                                    float *green_input,
                                    float *blue_input,
                                    float *red_output,
                                    float *green_output,
                                    float *blue_output,
                                    int    width,
                                    int    height,
                                    float dt,
                                    int Niter)
{
  int i;

  /* Canales auxiliares para la entrada de cada iteracion */
  float *red   = (float*)malloc (sizeof(float) * width * height);
  float *green = (float*)malloc (sizeof(float) * width * height);
  float *blue  = (float*)malloc (sizeof(float) * width * height);

  /* Canal de la iteracion anterior a la actual (input-1) */ 
  float *red_anterior   = (float*)malloc (sizeof(float) * width * height);
  float *green_anterior = (float*)malloc (sizeof(float) * width * height);
  float *blue_anterior  = (float*)malloc (sizeof(float) * width * height);

  memcpy (red,   red_input,   sizeof(float) * width * height);
  memcpy (green, green_input, sizeof(float) * width * height);
  memcpy (blue,  blue_input,  sizeof(float) * width * height);

  memcpy (red_anterior,   red_input,   sizeof(float) * width * height);
  memcpy (green_anterior, green_input, sizeof(float) * width * height);
  memcpy (blue_anterior,  blue_input,  sizeof(float) * width * height);


  for (i=0; i<Niter; i++)
  {
    unsigned char *ured, *ugreen, *ublue;
    char imagen[100];

    /* Llevamos a cabo la iteracion para cada canal*/
    aan_ondas_un_canal (red,   red_output,   red_anterior,   width, height, dt);
    aan_ondas_un_canal (green, green_output, green_anterior, width, height, dt);
    aan_ondas_un_canal (blue,  blue_output,  blue_anterior,  width, height, dt);

    /* Copiamos el canal de entrada como canal anterior */
    memcpy (red_anterior,   red,   sizeof(float) * width * height);
    memcpy (green_anterior, green, sizeof(float) * width * height);
    memcpy (blue_anterior,  blue,  sizeof(float) * width * height);

    /* Guardamos el resultado en un fichero */
    sprintf(imagen, "imagen_1%05d.bmp", i);
    
    ured   = float_to_uchar (red_output,   width*height);
    ugreen = float_to_uchar (green_output, width*height);
    ublue  = float_to_uchar (blue_output,  width*height);

    ami_write_bmp (imagen, ured, ugreen, ublue, width, height);

    free (ured); free (ugreen); free (ublue);

    /* Copiamos el canal de salida a los canales de entrada para la
     * siguiente iteracion */
    memcpy (red,   red_output,   sizeof(float) * width * height);
    memcpy (green, green_output, sizeof(float) * width * height);
    memcpy (blue,  blue_output,  sizeof(float) * width * height);
  }

  /* Liberamos la memoria de los canales auxiliares */
  free (red); free (green); free (blue);
  free (red_anterior); free (green_anterior); free (blue_anterior);
}
