#include <stdlib.h>
#include <stdio.h>

#include "ami_bmp.h"
#include "float_utils.h"
#include "aan_perona_malik.h"

#define NITERS 100
#define DT     0.3
#define DH     1.0

#ifdef P1
#define LAMBDA 0.01
#endif

#ifdef P2
#define LAMBDA 0.001
#endif

#ifdef P3
#define LAMBDA 0.0001
#endif

int
main (int argc, char **argv)
{
  int            w, h;
  unsigned char *red, *green, *blue;
  float         *fred, *fgreen, *fblue;
  float         *fred2, *fgreen2, *fblue2;

  if (argc < 2)
  {
    fprintf (stderr, "Usage: %s <BMP file>\n", argv[0]);
    return -1;
  }

  /* Leemos el fichero dado por el primer argumento */
  if (ami_read_bmp (argv[1], &red, &green, &blue, &w, &h) < 0)
    return -1;

  fred2   = (float*)malloc (sizeof (float) * w * h);
  fgreen2 = (float*)malloc (sizeof (float) * w * h);
  fblue2  = (float*)malloc (sizeof (float) * w * h);

  fred   = uchar_to_float (red,   w*h);
  fgreen = uchar_to_float (green, w*h);
  fblue  = uchar_to_float (blue,  w*h);

  aan_perona_malik (fred,  fgreen,  fblue,
                    fred2, fgreen2, fblue2,
                    w, h,
                    DT, DH, LAMBDA, NITERS);

  /* Liberamos la memoria de los canales */
  free (fred); free (fgreen); free (fblue);
  free (fred2); free (fgreen2); free (fblue2);
  free (red);  free (green);  free (blue);
  return 0;
}
