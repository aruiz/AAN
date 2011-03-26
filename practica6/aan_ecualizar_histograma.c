void
aan_ecualizar_histograma(float *h, float *e, int *f)
{
	int k;
	float sum_e = 0.0;
	
	for (k=0; k<256; k++)
	{
		int i;
		float sum_h = 0.0;
		
		/* Calculamos la sumatoria de  e[0..k] */
		sum_e = sum_e + e[k];
		
		/* Hayamos el valor de f[k] buscando un i para el que la sumatoria
		 * h[0..i] se aproxima a e[0..k] */
		for (i=0; i<256; i++)
		{
			if ((sum_h + h[i]) > sum_e)
				break;
			sum_h = sum_h + h[i];
		}
		
		/* Asignamos a f[k] la sumatoria resultante h[0..i] */
		f[k] = sum_h;
	}
}

void
aan_ecualizar_histograma_canal(float *canal_input,
                               float *canal_output,
                               int width,
                               int height,
                               int *f)
{
	int i;
	
	for (i=0; i<(width * height); i++)
	{
		int color = (int)(255.0 * canal_input[i]);
		canal_output[i] = f[color];
	}
}
