#include <stdlib.h>
#include <math.h>

/* La ventana siempre debe ser impar para que tenga un pixel central */
#define VENTANA 5

/* Umbral de tolerancia para la correlacion */
#define TOL 0.95

enum {
  COR_VERTICAL,
  COR_HORIZONTAL
};


void
copiar_ventana (float *input,
                float *output,
                int    width,
                int    height,
                int    i,
                int    j)
{
  int x, y;
  
  /* Movemos el origen a la esquina superior de la ventana */
  i = i - VENTANA / 2;
  j = j - VENTANA / 2;


  /* Copiamos los valores al buffer de destino */  
  for (y=0; y < VENTANA; y++)
    for (x=0; x < VENTANA; x++)
        output[y * VENTANA + x] = input[(j + y) * height + (i + x)];
}

void
buscar_correlacion (float *vent,
                    float *input,
                    float  media_input,
                    float  stddev_input,
                    int    width,
                    int    height,
                    int   *x,
                    int   *y)
{
  float corr;
  int i, j;
  
  float media_ventana = 0.0;
  
  for (i=0; i < VENTANA * VENTANA; i++)
  {
    media_ventana = media_ventana + vent[i];
  }
  media_ventana = media_ventana / (float)(VENTANA * VENTANA);
  
  for (i = VENTANA / 2; i < (width - VENTANA / 2); i++)
  {
    for (j = VENTANA / 2; j < (height - VENTANA / 2); j++)
    {
      
    }
  }
}

float*
aan_correlacion (float *a, float *b, int width, int height, int orientacion)
{
  int i, j;
  float *output = (float*)malloc (sizeof (float) * width * height);
  float *area   = (float*)malloc (sizeof (float) * VENTANA * VENTANA);
  float  stddev = 0.0;
  float  media  = 0.0;

  for (i=0; i < width * height; i++)
  {
    /* Ponemos todos los valores de la salida a 0 */
    output[i] = 0.0;
    
    /* Calculamos la media de la segunda imagen */
    media = media + b[i];
  }
  media = media / (float)(width * height);
  
  /* Calculamos la desviacion estandar de la segunda imagen */
  for (i=0; i < width * height; i++)
  {
    stddev = stddev + (b[i] + media)*(b[i] + media);
  }
  stddev = stddev / (width * height);
  stddev = sqrtf (stddev);

  /* Ignoramos los pixeles de los bordes por no poder llenar una ventana */
  for (i = VENTANA / 2; i < (width - VENTANA / 2); i++)
  {
    for (j = VENTANA / 2; j < (height - VENTANA / 2); j++)
    {
      int x, y;
      copiar_ventana (a, area, width, height, i, j);
      buscar_correlacion (area, b, media, stddev, width, height, &x, &y);
    }
  }

  free (area);
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
