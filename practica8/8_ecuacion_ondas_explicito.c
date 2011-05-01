#include "ami_bmp.h"
#include "aan_ondas.h"

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

  fred   = uchar_to_float (red);
  fgreen = uchar_to_float (green);
  fblue  = uchar_to_float (blue);

  aan_ecuacion_ondas_metodo_explícito(fred, fgreen, fblue,
                                      fred2, fgreen2, fblue2,
                                      w, h,
                                      0.1,
                                      1000);

  /* Liberamos la memoria de los canales */
  free (fred); free (fgreen); free (fblue);
  free (fred2); free (fgreen2); free (fblue2);
  free (red);  free (green);  free (blue);
  return 0;
}
