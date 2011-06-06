#include <stdlib.h>
#include <stdio.h>

#include "ami_bmp.h"
#include "float_utils.h"

int
main (int argc, char **argv)
{
  int            w, h;
  unsigned char *red1, *green1, *blue1;
  unsigned char *red2, *green2, *blue2;
  float         *fred1, *fgreen1, *fblue1;
  float         *fred2, *fgreen2, *fblue2;
  float         *fred_d, *fgreen_d, *fblue_d;
  
  if (argc < 3)
  {
    fprintf (stderr, "Usage: %s <BMP file 1> <BMP file 2>\n", argv[0]);
    return -1;
  }

  /* Leemos los ficheros dados como argumentos */
  if (ami_read_bmp (argv[1], &red1, &green1, &blue1, &w, &h) < 0)
    return -1;

  if (ami_read_bmp (argv[1], &red2, &green2, &blue2, &w, &h) < 0)
    return -1;

  fred1   = uchar_to_float (red1,   w*h);
  fgreen1 = uchar_to_float (green1, w*h);
  fblue1  = uchar_to_float (blue1,  w*h);

  fred2   = uchar_to_float (red2,   w*h);
  fgreen2 = uchar_to_float (green2, w*h);
  fblue2  = uchar_to_float (blue2,  w*h);
  
       
  
  /* Liberamos la memoria de los canales */
  free (fred1); free (fgreen1); free (fblue1);
  free (fred2); free (fgreen2); free (fblue2);
  free (red1);  free (green1);  free (blue1);
  free (red2);  free (green2);  free (blue2);
  return 0;
}
