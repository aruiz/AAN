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

  /* Primer cuadrante */
	for (i = 0; i < width; i++)
	{
		for (j=0; j < height; j++)
		{
		    int index = (width*2)*j + i;
		    
		    output[index] = c[width*j + i];
		}
  }
  
  /* Segundo cuadrante */
	for (i = 0; i < width; i++)
	{
		for (j=0; j < height; j++)
		{
		    int index = (width*2)*j + i + width;
		    
		    output[index] = d[width*j + i];
		}
  }
  
  /* Tercer cuadrante */
  for (i = 0; i < width; i++)
	{
		for (j=0; j < height; j++)
		{
		    int index = (height * width * 2) + (width*2)*j + i;
		    
		    output[index] = a[width*j + i];
		}
  }
  
  /* Cuarto cuadrante */
  for (i = 0; i < width; i++)
	{
		for (j=0; j < height; j++)
		{
		    int index = (height * width * 2) + (width*2)*j + i + width;
		    
		    output[index] = b[width*j + i];
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

  if (ami_read_bmp (argv[2], &red2, &green2, &blue2, &w2, &h2) < 0)
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

  fred_v   = (float*)malloc(sizeof (float) * w1 * h1);
  fgreen_v = (float*)malloc(sizeof (float) * w1 * h1);
  fblue_v  = (float*)malloc(sizeof (float) * w1 * h1);
  fred_h   = (float*)malloc(sizeof (float) * w1 * h1);
  fgreen_h = (float*)malloc(sizeof (float) * w1 * h1);
  fblue_h  = (float*)malloc(sizeof (float) * w1 * h1);

  aan_correlacion   (fgreen1, fblue2, w1, h1,   fblue_v,   fblue_h);

  fred_resultado =   unir_cuatro_imagenes (fred1,   fred2,   fblue_v, fblue_h,  w1, h1);
  fgreen_resultado = unir_cuatro_imagenes (fgreen1, fgreen2, fblue_v, fblue_h,  w1, h1);
  fblue_resultado =  unir_cuatro_imagenes (fblue1,  fblue2,  fblue_v, fblue_h,  w1, h1);

  red_resultado =   float_to_uchar (fred_resultado,   w1*2 * h1*2); 
  green_resultado = float_to_uchar (fgreen_resultado, w1*2 * h1*2);
  blue_resultado =  float_to_uchar (fblue_resultado,  w1*2 * h1*2);

	ami_write_bmp ("movimiento.bmp", red_resultado, green_resultado, blue_resultado, w1*2, h1*2);

  /* Liberamos la memoria de los canales */
  free (fred1); free (fgreen1); free (fblue1);
  free (fred2); free (fgreen2); free (fblue2);
  free (fred_v); free (fgreen_v); free (fblue_v);
  free (fred_h); free (fgreen_h); free (fblue_h);
  free (red1);  free (green1);  free (blue1);
  free (red2);  free (green2);  free (blue2);

  return 0;
}
