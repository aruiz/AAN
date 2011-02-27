float **
multiplicar_matrices (float **a, float **b)
{
	return 0;
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
	
	/* Area afectada por la mascara */
	float *area[3] = {(float*)malloc (sizeof(float) * 3),
	                  (float*)malloc (sizeof(float) * 3),
	                  (float*)malloc (sizeof(float) * 3)};

	for (j=1; j < height-1; j++)	
	{
		for (i=1; i < width-1; i++)
		{
			/* FIXME: Se esta descartando los pixeles de los bordes del canal */
			/* Copiamos el area afectada en una matriz */
			for (k=0; k < 3; k++)
				for (l=0; l < 3; l++)
					area[k][0] = (width * (j - 1 + l)) + (i - 1 + k);
					
			
		}		
	}
	
	free (area[0]); free (area[1]); free (area[2]);
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
	return 0;
}
