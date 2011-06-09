#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "ami.h"
#include "aan_mascara.h"


/* Utilizamos una mascara laplaciana como aproximacion al modulo del gradiente */
void*
modgradiente (float *input, int width, int height)
{
	float  *mod;
	float **lap;

	mod = (float*)malloc (sizeof(float) * width * height);
	ami_malloc2d (lap, float, 3, 3);

	lap[0][0] = 1.0; lap[0][1] =  1.0; lap[0][2] = 1.0;
	lap[1][0] = 1.0; lap[1][1] = -8.0; lap[1][2] = 1.0;
	lap[2][0] = 1.0; lap[2][1] =  1.0; lap[2][2] = 1.0;

	aan_mascara_canal (input, mod, width, height, lap);

	return mod;
}



void
aan_propagacion_canal (float *input,
                       float *output,
                       int    width,
                       int    height,
                       float  dt,
                       float *F)
{
	int i, j;
	
	float *mod = modgradiente (input, width, height);
	
	for (i=0; i<height; i++)
	{
		for (j=0; j<width; j++)
		{
			output[i*width + j] = input[i*width + j] + F[i*width + j] * dt * mod[i*width + j];
			
			if (output[i*width + j] < 0.0)
				output[i*width + j] = 0.0;
			if (output[i*width + j] > 1.0)
				output[i*width + j] = 1.0;
		}
	}
}

void aan_ecuacion_propagacion_frentes(float *red_input,
                                      float *green_input,
                                      float *blue_input,
                                      float *red_output,
                                      float *green_output,
                                      float *blue_output,
                                      int width,
                                      int height,
                                      float dt,
                                      int Niter,
                                      float *F)
{
	int i;

	float *red_tmp   = (float*)malloc (sizeof(float) * width * height);
	
	memcpy (red_tmp, red_input, sizeof(float) * width * height);

	memcpy (green_output, green_input, sizeof(float) * width * height);
	memcpy (blue_output,  blue_input,  sizeof(float) * width * height);
	
	for (i=0; i<Niter; i++)
	{
		aan_propagacion_canal (red_tmp, red_output, width, height, dt, F);

		memcpy (red_tmp,   red_output,   sizeof(float) * width * height);
	}

	/* Liberamos memoria */	
	free (red_tmp);
}
