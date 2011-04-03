void aan_propagacion_canal (float *input,
                            float *output,
                            int width,
                            int height,
                            float dt,
                            float *F)
{

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
