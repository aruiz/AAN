#include <stdlib.h>
#include <stdio.h>

#include "ami_bmp.h"
#include "float_utils.h"

#include "aan_correlacion.h"

float*
unir_cuatro_imagenes (float *a, float *b, float*c, float *d, int width, int height)
{
  int i, j;
  float *output = (float*)malloc (sizeof (float) * (width * 2) * (height * 2));
  
  float *tmp = output + (width * 2)*height;
  
  	
	for (i = 0; i < width*2; i++)
	{
		for (j=0; j < height; j++)
		{
			/* Indice del pixel del canal de slida */
			int index = (width*2) * j + i;
			
			/* Primer y tercer canal */
			if (i<width)
			{
				output[index] = a[width * j + i];
				tmp[index] = c[width * j + i];
		  }
			/* Segundo y cuarto canal */
			else
			{
				output[index] = b[width * j + i - width];
				tmp[index]    = c[width * j + i - width];
		  }
		}
	}
  
  return output;
}

int
main (int argc, char **argv)
{
  int            i;
  int            w1, h1, w2, h2;
  unsigned char *red1, *green1, *blue1;
  unsigned char *red2, *green2, *blue2;
  float         *fred1, *fgreen1, *fblue1;
  float         *fred2, *fgreen2, *fblue2;

  float         *fred_v, *fgreen_v, *fblue_v;
  float         *fred_h, *fgreen_h, *fblue_h;
  
  float         *fred_resultado,
                *fgreen_resultado,
                *fblue_resultado;
  unsigned char *red_resultado,
                *green_resultado,
                *blue_resultado;

  if (argc < 3)
  {
    fprintf (stderr, "Usage: %s <BMP file 1> <BMP file 2>\n", argv[0]);
    return -1;
  }

  /* Leemos los ficheros dados como argumentos */
  if (ami_read_bmp (argv[1], &red1, &green1, &blue1, &w1, &h1) < 0)
    return -1;

  if (ami_read_bmp (argv[1], &red2, &green2, &blue2, &w2, &h2) < 0)
    return -2;
    
  if (w1 != w2 || h1 != h2)
  {
    fprintf (stderr, "Las imagenes son de dimensiones distintas");
    return -3;
  }

  fred1   = uchar_to_float (red1,   w1*h1);
  fgreen1 = uchar_to_float (green1, w1*h1);
  fblue1  = uchar_to_float (blue1,  w1*h1);

  fred2   = uchar_to_float (red2,   w2*h2);
  fgreen2 = uchar_to_float (green2, w2*h2);
  fblue2  = uchar_to_float (blue2,  w2*h2);

/*
  fprintf (stderr, "Correlaciones verticales: rojo\n");
  fred_v  = aan_correlacion_vertical   (fred1, fred2, w1, h1);
  fprintf (stderr, "Correlaciones verticales: verde\n");
  fgreen_v  = aan_correlacion_vertical   (fgreen1, fgreen2, w1, h1);
  fprintf (stderr, "Correlaciones verticales: azul\n");
  fblue_v  = aan_correlacion_vertical   (fblue1, fblue2, w1, h1);

  fprintf (stderr, "Correlaciones horizontales: azul\n");
  fred_h  = aan_correlacion_horizontal (fred1, fred2, w1, h1);
  fprintf (stderr, "Correlaciones horizontales: verde\n");
  fgreen_h  = aan_correlacion_horizontal (fgreen1, fgreen2, w1, h1);
  fprintf (stderr, "Correlaciones horizontales: azul\n");
  fblue_h  = aan_correlacion_horizontal (fblue1, fblue2, w1, h1);

  for (i=0; i<w1*h1; i++)
  {
    if (fred_v[i]   > 0.0 ||
        fgreen_v[i] > 0.0 ||
        fblue_v[i]  > 0.0)
    {
      fred_v[i]   = fred2[i];
      fgreen_v[i] = fgreen2[i];
      fblue_v[i]  = fblue2[i];
    }

    if (fred_h[i]   > 0.0 ||
        fgreen_h[i] > 0.0 ||
        fblue_h[i]  > 0.0)
    {
      fred_h[i]   = fred2[i];
      fgreen_h[i] = fgreen2[i];
      fblue_h[i]  = fblue2[i];
    }
  }
 */

  fred_resultado =   unir_cuatro_imagenes (fred1, fred2, fred1, fred1, w1, h1);
  fgreen_resultado = unir_cuatro_imagenes (fgreen1, fgreen2, fgreen1, fgreen1, w1, h1);
  fblue_resultado =  unir_cuatro_imagenes (fblue1, fblue2, fblue1, fblue1, w1, h1);
  
  red_resultado =   float_to_uchar (fred_resultado,   w1*2 * h1*2); 
  green_resultado = float_to_uchar (fgreen_resultado, w1*2 * h1*2);
  blue_resultado =  float_to_uchar (fblue_resultado,  w1*2 * h1*2);
  
	ami_write_bmp ("movimiento.bmp", red_resultado, green_resultado, blue_resultado, w1*2, h1*2);
  
  /* Liberamos la memoria de los canales */
  free (fred1); free (fgreen1); free (fblue1);
  free (fred2); free (fgreen2); free (fblue2);
/*  free (fred_v); free (fgreen_v); free (fblue_v);
  free (fred_h); free (fgreen_h); free (fblue_h);*/
  free (red1);  free (green1);  free (blue1);
  free (red2);  free (green2);  free (blue2);
  return 0;
}
