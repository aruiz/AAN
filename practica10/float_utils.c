#include <stdlib.h>

float*
uchar_to_float (unsigned char *canal, size_t pixeles)
{
	int i;
	float * output = (float*) malloc (sizeof (float) * pixeles);
	for (i=0; i<pixeles; i++)
	{
		/* Pasamos el valor a precision flotante: (0-255) -> (0.0-1.0) */
		output[i] = canal[i] / 255.0;
	}
	return output;
}

unsigned char*
float_to_uchar (float *canal, size_t pixeles)
{
	int i;
	unsigned char * output = (unsigned char*) malloc (sizeof (unsigned char) * pixeles);
	for (i=0; i<pixeles; i++)
	{
		/* Pasamos el valor a precision entera: (0.0-1.0) -> (0-255) */
		float tmp = canal[i];
    if (tmp < 0.0)
      tmp = 0.0;
		output[i] = (int)(canal[i] * 255.0);
	}
	return output;
}
