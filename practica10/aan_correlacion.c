#include <stdlib.h>

#define VENTANA 5

enum {
  COR_VERTICAL,
  COR_HORIZONTAL
};

float*
aan_correlacion (float *a, float *b, int width, int height, int orientacion)
{
  int i;
  float *output = (float*)malloc (sizeof (float) * width * height);

  for (i=0; i < width * height; i++)
    output[i] = 0.0;
  
  /* Ignoramos las areas que no ocupan una ventana entera */
  width = width - (width % VENTANA);
  height = height - (height % VENTANA);
  
  return output;
}

float*
aan_correlacion_vertical (float *a, float *b, int width, int height)
{
  return aan_correlacion (a, b, width, height, COR_VERTICAL);
}

float*
aan_correlacion_horizontal (float *a, float *b, int width, int height)
{
  return aan_correlacion (a, b, width, height, COR_HORIZONTAL);
}
