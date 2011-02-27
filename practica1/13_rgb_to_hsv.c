#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ami.h"
#include "ami_bmp.h"

#include "aan_unir_canales.h"

typedef enum 
{
	MAX_RED,
	MAX_GREEN,
	MAX_BLUE
} CanalMaximo;

void
aan_rgb_to_hsv (unsigned char  *red,
                unsigned char  *green,
                unsigned char  *blue,
                unsigned char **hue,
                unsigned char **saturation,
                unsigned char **value,
                          int   width,
                          int   height)
{
	int i;
	unsigned char *h, *s, *v;
	
	/* Reservamos la memoria necesaria para los nuevos canales HSV */
	h = *hue        = (unsigned char*) malloc (sizeof (unsigned char) * width * height);
	s = *saturation = (unsigned char*) malloc (sizeof (unsigned char) * width * height);
	v = *value      = (unsigned char*) malloc (sizeof (unsigned char) * width * height);
	
	for (i=0; i < (width * height); i++)
	{
		CanalMaximo max;
		unsigned char min = 255;
		
		/* Buscamos el valor maximo, Canal V */
		v[i] = 0;
		if (red[i] > v[i])
		{
			v[i] = red[i];
			max = MAX_RED;
		}
		if (green[i] > v[i])
		{
			v[i] = green[i];
			max = MAX_GREEN;
		}
		if (blue[i] > v[i])
		{
			v[i] = blue[i];
			max = MAX_BLUE;
		}
			
		/* buscamos el valor minimo */
		if (red[i] < min)
			min = red[i];
		if (green[i] < min)
			min = green[i];
		if (blue[i] < min)
			min = blue[i];

		/* Canal S */
		s[i] = v[i] - min;
		
		/* Canal H */
		if (max == MAX_RED)
			h[i] = (unsigned char)(43.0 * (float)(green[i] - blue[i]) / (float)(s[i])) % 256;
			
		else if (max == MAX_GREEN)
			h[i] = (unsigned char)(43.0 * (float)(blue[i] - red[i])   / (float)(s[i])) + 85;
			
		else if (max == MAX_BLUE)
			h[i] = (unsigned char)(43.0 * (float)(red[i] - green[i])  / (float)(s[i])) + 170;
	}
	
	return;
}

int
main (int argc, char** argv)
{
	int w, h;
	unsigned char *red1, *green1, *blue1,
	              *red2, *green2, *blue2,
	              *red3, *green3, *blue3,
	              *red4, *green4, *blue4,
	              *red5, *green5, *blue5,
	              *hue,  *saturation, *value;

	if (argc < 2)
	{
		fprintf (stderr, "Usage: %s <BMP file>\n", argv[0]);
		return -1;
	}

	/* Leemos el fichero dado por el primer argumento */	
	if (ami_read_bmp (argv[1], &red1, &green1, &blue1, &w, &h) < 0)
		return -1;

	aan_rgb_to_hsv (red1, green1, blue1, &hue, &saturation, &value, w, h);
	
	/* Unimos cada canal con la imagen original */
	aan_unir_canales_unsigned_char (red1,   hue, &red2,   w, h);
	aan_unir_canales_unsigned_char (green1, hue, &green2, w, h);
	aan_unir_canales_unsigned_char (blue1,  hue, &blue2,  w, h);

	aan_unir_canales_unsigned_char (red1,   saturation, &red3,   w, h);
	aan_unir_canales_unsigned_char (green1, saturation, &green3, w, h);
	aan_unir_canales_unsigned_char (blue1,  saturation, &blue3,  w, h);

	aan_unir_canales_unsigned_char (red1,   value, &red4,   w, h);
	aan_unir_canales_unsigned_char (green1, value, &green4, w, h);
	aan_unir_canales_unsigned_char (blue1,  value, &blue4,  w, h);

	/* Creamos una imagen resultante con todas las comparativas anteriores */
	w = w*2 + 4;

	red5   = malloc (sizeof (unsigned char) * w * h * 3);
	green5 = malloc (sizeof (unsigned char) * w * h * 3);
	blue5  = malloc (sizeof (unsigned char) * w * h * 3);

	/* Unimos los canales rojos */
	memcpy ((void*)red5, (const void*)red2, w * h);
	memcpy ((void*)(red5 + w*h), (const void*)red3, w * h);
	memcpy ((void*)(red5 + w*h*2), (const void*)red4, w * h);

	/* Unimos los canales verdes */
	memcpy ((void*)green5, (const void*)green2, w * h);
	memcpy ((void*)(green5 + w*h), (const void*)green3, w * h);
	memcpy ((void*)(green5 + w*h*2), (const void*)green4, w * h);

	/* Unimos los canales azules */
	memcpy ((void*)blue5, (const void*)blue2, w * h);
	memcpy ((void*)(blue5 + w*h), (const void*)blue3, w * h);
	memcpy ((void*)(blue5 + w*h*2), (const void*)blue4, w * h);
	
	ami_write_bmp ("./13_rgb_to_hsv.bmp", red5, green5, blue5, w, h*3);

	free (red1); free (green1); free (blue1);
	free (red2); free (green2); free (blue2);
	free (red3); free (green3); free (blue3);
	free (red4); free (green4); free (blue4);
	free (red5); free (green5); free (blue5);
	free (hue); free (saturation); free (value);	
	return 0;
}
