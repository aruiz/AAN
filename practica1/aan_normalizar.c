#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
aan_normalizar_canal_unsigned_char (unsigned char *canal_input,
                                    unsigned char *canal_output,
                                    int width,
                                    int height)
{
	int i;
	unsigned char max = 0, min = 255;
	
	if (!canal_input || !canal_output)
	{
		fprintf (stderr, "Ninguno de los canales puede ser NULL");
		return;
	}
	
	if (width < 1 || height < 1)
	{
		fprintf (stderr, "La anchura y la altura del canal deben ser mayores que 0");
		return;
	}
	
	for (i=0; i < (width * height); i++)
	{
		if (canal_input[i] > max)
			max = canal_input[i];
		if (canal_input[i] < min)
			min = canal_input[i];
	}

	for (i=0; i < (width * height); i++)
	{
		/* Hacemos la operacion con precision flotante y luego la pasamos a unsigned char */
		canal_output[i] = (unsigned char)(255.0 * (float)(canal_input[i] - min) / (float)(max-min));
	}
}
                                    

void
aan_normalizar_imagen_unsigned_char (unsigned char *red,
                                     unsigned char *green,
                                     unsigned char *blue,
                                               int  width,
                                               int height)
{
	if (!red || !green || !blue)
	{
		fprintf (stderr, "Ninguno de los canales puede ser NULL");
		return;
	}
	
	if (width < 1 || height < 1)
	{
		fprintf (stderr, "La anchura y la altura del canal deben ser mayores que 0");
		return;
	}

	unsigned char *tmp = (unsigned char *) malloc (sizeof (unsigned char) * width * height);
	
	aan_normalizar_canal_unsigned_char (red, tmp, width, height);
	memcpy ((void*)red, (const void*)tmp, sizeof (unsigned char) * width * height);
	
	aan_normalizar_canal_unsigned_char (green, tmp, width, height);
	memcpy ((void*)green, (const void*)tmp, sizeof (unsigned char) * width * height);
	
	aan_normalizar_canal_unsigned_char (blue, tmp, width, height);
	memcpy ((void*)blue, (const void*)tmp, sizeof (unsigned char) * width * height);
	
	free (tmp);	
}

void
aan_normalizar_canal_float (float *canal_input,
                            float *canal_output,
                            int width,
                            int height)
{
	int i;
	unsigned char max = 0, min = 255;
	
	if (!canal_input || !canal_output)
	{
		fprintf (stderr, "Ninguno de los canales puede ser NULL");
		return;
	}
	
	if (width < 1 || height < 1)
	{
		fprintf (stderr, "La anchura y la altura del canal deben ser mayores que 0");
		return;
	}
	
	for (i=0; i < (width * height); i++)
	{
		if (canal_input[i] > max)
			max = canal_input[i];
		if (canal_input[i] < min)
			min = canal_input[i];
	}

	for (i=0; i < (width * height); i++)
	{
		/* Hacemos la operacion con precision flotante y luego la pasamos a unsigned char */
		canal_output[i] = (unsigned char)(255.0 * (float)(canal_input[i] - min) / (float)(max-min));
	}
}
                                    

void
aan_normalizar_imagen_float (float *red,
                             float *green,
                             float *blue,
                             int  width,
                             int height)
{
	if (!red || !green || !blue)
	{
		fprintf (stderr, "Ninguno de los canales puede ser NULL");
		return;
	}
	
	if (width < 1 || height < 1)
	{
		fprintf (stderr, "La anchura y la altura del canal deben ser mayores que 0");
		return;
	}

	float *tmp = (float *) malloc (sizeof (float) * width * height);
	
	aan_normalizar_canal_float (red, tmp, width, height);
	memcpy ((void*)red, (const void*)tmp, sizeof (float) * width * height);
	
	aan_normalizar_canal_float (green, tmp, width, height);
	memcpy ((void*)green, (const void*)tmp, sizeof (float) * width * height);
	
	aan_normalizar_canal_float (blue, tmp, width, height);
	memcpy ((void*)blue, (const void*)tmp, sizeof (float) * width * height);
	
	free (tmp);	
}

