#include <math.h>

void*
gradiente_horizontal (float *input, int width, int height)
{
	float  *u_x;
	float **mu_x;

	u_x = (float*)malloc (sizeof(float) * width * height);
	ami_malloc2d (mu_x, float, 3, 3);

	/* Gradiente horizontal */
	u_x[0][0] = 0.25 * -(2.0 - sqrt(2.0));     u_x[0][1] = 0; u_x[0][2] = 0.25 * (2.0 - sqrt(2.0));
	u_x[1][0] = 0.25 * -2.0 * (sqrt(2.0) - 1); u_x[1][1] = 0; u_x[1][2] = 0.25 * 2.0 * (sqrt(2.0) - 1);
	u_x[2][0] = 0.25 * -(2.0 - sqrt(2.0));     u_x[2][1] = 0; u_x[2][2] = 0.25 * (2.0 - sqrt(2.0));

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
	u_y[0][0] = 0.25 * -(2.0 - sqrt(2.0)); u_y[0][1] = 0.25 * -2.0 * (sqrt(2.0) - 1); u_y[0][2] = 0.25 * -(2.0-sqrt(2.0));
	u_y[1][0] = 0;                         u_y[1][1] = 0;                             u_y[1][2] = 0;
	u_y[2][0] = 0.25 *  (2.0 - sqrt(2.0)); u_y[2][1] = 0.25 *  2.0 * (sqrt(2.0) - 1); u_y[2][2] = 0.25 *  (2.0 - sqrt(2.0));

	aan_mascara_canal (input, u_y, width, height, mu_y);

	ami_free2d (mu_y);
	return u_y;
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
	
	/* Hallar u_x y u_y */
	float *u_x = gradiente_vertical   (input, width, height);
	float *u_y = gradiente_horizontal (input, width, height);
	
	for (i=0; i<height; i++)
	{
		for (j=0; j<width; j++)
		{
			float potencia_u_x = powf (u_x[i*width + j], 2);
			float potencia_u_y = powf (u_y[i*width + j], 2);
			float raiz_suma = sqrtf (potencia_u_y + potencia_u_x);
			
			output[i*width + j] = input[i*width + j] + F[i*width + j] * dt * raiz_suma;
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

	memcpy (red_tmp,   red_input,   sizeof(float) * width * height);
	memcpy (green_tmp, green_input, sizeof(float) * width * height);
	memcpy (blue_tmp,  blue_input,  sizeof(float) * width * height);

	for (i=0; i<Niter; i++)
	{
		aan_propagacion_canal (red_tmp,   red_output,   width, height, dt, F);
		aan_propagacion_canal (green_tmp, green_output, width, height, dt, F);
		aan_propagacion_canal (blue_tmp,  blue_output,  width, height, dt, F);

		memcpy (red_tmp,   red_output,   sizeof(float) * width * height);
		memcpy (green_tmp, green_output, sizeof(float) * width * height);
		memcpy (blue_tmp,  blue_output,  sizeof(float) * width * height);
	}

	/* Liberamos memoria */	
	free (red_tmp); free (green_tmp); free (blue_tmp);
}
