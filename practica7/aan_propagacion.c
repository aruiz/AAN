#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "ami.h"
#include "aan_mascara.h"

void*
gradiente_horizontal (float *input, int width, int height)
{
	float  *u_x;
	float **mu_x;

	u_x = (float*)malloc (sizeof(float) * width * height);
	ami_malloc2d (mu_x, float, 3, 3);

	/* Gradiente horizontal */
	mu_x[0][0] = 0.25 * -(2.0 - sqrt(2.0));     mu_x[0][1] = 0; mu_x[0][2] = 0.25 * (2.0 - sqrt(2.0));
	mu_x[1][0] = 0.25 * -2.0 * (sqrt(2.0) - 1); mu_x[1][1] = 0; mu_x[1][2] = 0.25 * 2.0 * (sqrt(2.0) - 1);
	mu_x[2][0] = 0.25 * -(2.0 - sqrt(2.0));     mu_x[2][1] = 0; mu_x[2][2] = 0.25 * (2.0 - sqrt(2.0));

	aan_mascara_canal (input, u_x, width, height, mu_x);

	ami_free2d (mu_x);
	return u_x;
}

void*
gradiente_vertical (float *input, int width, int height)
{
	float  *u_y;
	float **mu_y;

	u_y = (float*)malloc (sizeof(float) * width * height);
	ami_malloc2d (mu_y, float, 3, 3);
	
	/* Gradiente vertical */
	mu_y[0][0] = 0.25 * -(2.0 - sqrt(2.0)); mu_y[0][1] = 0.25 * -2.0 * (sqrt(2.0) - 1); mu_y[0][2] = 0.25 * -(2.0-sqrt(2.0));
	mu_y[1][0] = 0;                         mu_y[1][1] = 0;                             mu_y[1][2] = 0;
	mu_y[2][0] = 0.25 *  (2.0 - sqrt(2.0)); mu_y[2][1] = 0.25 *  2.0 * (sqrt(2.0) - 1); mu_y[2][2] = 0.25 *  (2.0 - sqrt(2.0));

	aan_mascara_canal (input, u_y, width, height, mu_y);

	ami_free2d (mu_y);
	return u_y;
}

void*
modgradiente (float *input, int width, int height)
{
	int i;
	float *u_x = gradiente_horizontal (input, width, height);
	float *u_y = gradiente_vertical   (input, width, height);
	float *frente = (float*)malloc (sizeof(float) * width * height);
	
	for (i=0; i < (width * height); i++)
	{
			float potencia_u_x = powf (u_x[i], 2);
			float potencia_u_y = powf (u_y[i], 2);
			frente[i] = sqrtf (potencia_u_y + potencia_u_x);
	}
	
	free (u_x); free (u_y);
	return frente;
}

void
aan_propagacion_canal (float *input,
                       float *output,
                       float *frente,
                       int    width,
                       int    height,
                       float  dt,
                       float *F)
{
	int i, j;
	
	for (i=0; i<height; i++)
	{
		for (j=0; j<width; j++)
		{
			output[i*width + j] = input[i*width + j] + F[i*width + j] * dt * frente[i*width + j];
			
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
	float *green_tmp = (float*)malloc (sizeof(float) * width * height);
	float *blue_tmp  = (float*)malloc (sizeof(float) * width * height);

	float *frente    = modgradiente (red_input, width, height);
	
	memcpy (red_tmp,   red_input,   sizeof(float) * width * height);
	memcpy (green_tmp, green_input, sizeof(float) * width * height);
	memcpy (blue_tmp,  blue_input,  sizeof(float) * width * height);

	for (i=0; i<Niter; i++)
	{
		aan_propagacion_canal (red_tmp,   red_output,   frente, width, height, dt, F);
		aan_propagacion_canal (green_tmp, green_output, frente, width, height, dt, F);
		aan_propagacion_canal (blue_tmp,  blue_output,  frente, width, height, dt, F);

		memcpy (red_tmp,   red_output,   sizeof(float) * width * height);
		memcpy (green_tmp, green_output, sizeof(float) * width * height);
		memcpy (blue_tmp,  blue_output,  sizeof(float) * width * height);
	}

	/* Liberamos memoria */	
	free (red_tmp); free (green_tmp); free (blue_tmp);
}
