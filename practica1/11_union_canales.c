#include <stdlib.h>

#include "ami.h"
#include "ami_bmp.h"

void
aan_unir_canales_unsigned_char (unsigned char  *canal1,
                                unsigned char  *canal2,
                                unsigned char **canal_output,
                                          int   width,
                                          int   height)
{
	unsigned char *output;
	int i, j, canal_size;
	
	if (!canal1 || !canal2 || !canal_output)
		return;
	if (width < 1 && height < 1)
		return;

	/* Reservamos la memoria del nuevo canal */
	canal_size = sizeof (unsigned char) * (width*2+4) * height;
	output = *canal_output = (unsigned char*) malloc (canal_size);
	
	for (i = 0; i < width*2 + 4; i++)
	{
		for (j=0; j < height; j++)
		{
			/* Indice del pixel del canal de slida */
			int index = (width*2 + 4) * j + i;
			
			/* Primer canal */
			if (i<width)
				output[index] = canal1[i];

			/* Pixeles negros */
			else if (i < width+4)
				output[index] = 0;
				
			/* Segundo canal */
			else
				output[index] = canal2[i - (width+4)];
		}
	}
	
	return;
}

int
main (int argc, char** argv)
{
	
	return 0;
}
