#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* La ventana siempre debe ser impar para que tenga un pixel central */
#define VENTANA 7

/* Umbral de tolerancia para la correlacion */
#define TOL 0.975

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

float
media (float *input, int length)
{
  int i;
  float m = 0.0;
  
  for (i=0; i < length; i++)
    m = m + input[i];

  m = m / (float)(length);

  return m;
}

float
stddev (float *input, int length, float avg)
{
  int i;
  float sd = 0.0;
  
  for (i=0; i < length; i++)
    sd = sd + (input[i] - avg)*(input[i] - avg);
    
  sd = sd / (float)length;
  sd = sqrtf (sd);
  
  return sd;
}




void
buscar_correlacion (float *vent,
                    float *input,
                    int    width,
                    int    height,
                    int    vent_i,
                    int    vent_j,
                    int   *x,
                    int   *y)
{
  float corr = -1.1;
  int i, j, k;

  float *area   = (float*)malloc (sizeof (float) * VENTANA * VENTANA);
  
  float media_ventana = media (vent, VENTANA * VENTANA);
  float stddev_ventana = stddev (vent, VENTANA * VENTANA, media_ventana);

  for (i = VENTANA / 2; i < (width - VENTANA / 2); i++)
  {
    for (j = VENTANA / 2; j < (height - VENTANA / 2); j++)
    {
      float sum, media_area, stddev_area;
      
      copiar_ventana (input, area, width, height, i, j);
      
      media_area = media (area, VENTANA * VENTANA);
      stddev_area = stddev (area, VENTANA * VENTANA, media_area);
      
      sum = 0.0;
      for (k=0; k<VENTANA*VENTANA; k++)
      {
        float tmp = (area[k] - media_area)*(vent[k] - media_ventana);
        tmp = tmp / (stddev_area*stddev_ventana);
        sum = sum + tmp;
      }
        
      sum = sum / (VENTANA*VENTANA - 1);
      
      if (sum < TOL)
        continue;

      if (sum < corr)
        continue;
      
      /* Calculamos ambas distancias */
      if (sum == corr)
      {
        float ab = sqrtf ((float) ((vent_i - i)*(vent_i - i) + (vent_j - j)*(vent_j - j)));
        float ac = sqrtf ((float) ((vent_i - *x)*(vent_i - *x) + (vent_j - *y)*(vent_j - *y)));
        
        /* Si la distancia del candidato es menor que la actual lo descartamos */
        if (ac > ab)
          continue;
      }

      corr = sum;
      *x = i;
      *y = j;
    }
  }
  
  free (area);
}

float*
aan_correlacion (float *a, float *b, int width, int height, int orientacion)
{
  int i, j;
  float *output = (float*)malloc (sizeof (float) * width * height);
  float *area   = (float*)malloc (sizeof (float) * VENTANA * VENTANA);

  for (i=0; i < width * height; i++)
  {
    /* Ponemos todos los valores de la salida a 0 */
    output[i] = 0.0;
  }

  /* Ignoramos los pixeles de los bordes por no poder llenar una ventana */
  for (i = VENTANA / 2; i < (width - VENTANA / 2); i++)
  {
    for (j = VENTANA / 2; j < (height - VENTANA / 2); j++)
    {
      int x = -1, y = -1;
      copiar_ventana (a, area, width, height, i, j);
      buscar_correlacion (area, b, width, height, i, j, &x, &y);
      
      printf ("%d %d\n", i, j);
      
      /* Si no hallamos correlacion continuamos */
      if (x == -1 || y == -1)
        continue;
      /* Si la correlacion nos devuelve el pixel original lo descartamos */
      if (x == i && y == j)
        continue;
      
      if (orientacion == COR_VERTICAL && (j - y) != 0)
        output[y * width + x] = b[y * width + x];
      else if ((i - x) != 0)
        output[y * width + x] = b[y * width + x];;
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
