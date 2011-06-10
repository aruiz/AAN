#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "ami.h"
#include "aan_mascara.h"


/* Utilizamos una mascara laplaciana como aproximacion al modulo del gradiente */
float*
grad_horizontal (float *input, int width, int height)
{
	float  *grad;
	float **u_x;

	grad = (float*)malloc (sizeof(float) * width * height);
	ami_malloc2d (u_x, float, 3, 3);

	u_x[0][0] = 0.25 * -(2.0 - sqrt(2.0));     u_x[0][1] = 0; u_x[0][2] = 0.25 * (2.0 - sqrt(2.0));
	u_x[1][0] = 0.25 * -2.0 * (sqrt(2.0) - 1); u_x[1][1] = 0; u_x[1][2] = 0.25 * 2.0 * (sqrt(2.0) - 1);
	u_x[2][0] = 0.25 * -(2.0 - sqrt(2.0));     u_x[2][1] = 0; u_x[2][2] = 0.25 * (2.0 - sqrt(2.0));

/*	u_x[0][0] = -3.0;     u_x[0][1] = 0; u_x[0][2] = 3.0;
	u_x[1][0] = -10.0;     u_x[1][1] = 0; u_x[1][2] = 10.0;
	u_x[2][0] = -3.0;     u_x[2][1] = 0; u_x[2][2] = 3.0;*/

	aan_mascara_canal (input, grad, width, height, u_x);

	ami_free2d(u_x);
	return grad;
}

float*
grad_vertical (float *input, int width, int height)
{
	float  *grad;
	float **u_y;

	grad = (float*)malloc (sizeof(float) * width * height);
	ami_malloc2d (u_y, float, 3, 3);

	u_y[0][0] = 0.25 * -(2.0 - sqrt(2.0)); u_y[0][1] = 0.25 * -2.0 * (sqrt(2.0) - 1); u_y[0][2] = 0.25 * -(2.0-sqrt(2.0));
	u_y[1][0] = 0;                         u_y[1][1] = 0;                             u_y[1][2] = 0;
	u_y[2][0] = 0.25 *  (2.0 - sqrt(2.0)); u_y[2][1] = 0.25 *  2.0 * (sqrt(2.0) - 1); u_y[2][2] = 0.25 *  (2.0 - sqrt(2.0));

/*	u_y[0][0] = -3.0; u_y[0][1] = -10.0; u_y[0][2] = -3.0;
	u_y[1][0] = 0;                         u_y[1][1] = 0;                             u_y[1][2] = 0;
	u_y[2][0] = 3.0; u_y[2][1] = 10.0; u_y[2][2] = 3.0;*/
	
	aan_mascara_canal (input, grad, width, height, u_y);

  ami_free2d(u_y);
	return grad;
}

float*
modgradiente (float *input, int width, int height)
{
  int i;
  float *mod, *gvert, *ghor;
  mod = (float*)malloc (sizeof(float) * (width * height));
  
  gvert = grad_vertical (input, width, height);
  ghor  = grad_horizontal (input, width, height); 
  
  for (i=0; i<(width*height); i++)
  {
    mod[i] = sqrtf((gvert[i]*gvert[i]) + (ghor[i] * ghor[i]));
  }
  
  free (gvert);
  free (ghor);
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
//			output[i*width + j] = mod[i*width + j];
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
	
  for (i=0; i<(width*height); i++)
    red_tmp[i] = red_input[i];
    
	memcpy (green_output, green_input, sizeof(float) * width * height);
	memcpy (blue_output,  blue_input,  sizeof(float) * width * height);
	
	for (i=0; i<Niter; i++)
	{
	  int j;
		aan_propagacion_canal (red_tmp, red_output, width, height, dt, F);

    for (j=0; j<width*height; j++)
      red_tmp[j] = red_output[j];
	}

	/* Liberamos memoria */	
	free (red_tmp);
}
