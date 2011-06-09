#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "aan_mascara.h"
#include "ami.h"

/* La ventana siempre debe ser impar para que tenga un pixel central */
#define VENTANA 9

/* La ventana de busqueda, tambien ha de ser impar */
#define BUSQUEDA 99

/* Umbral de tolerancia para la correlacion */
#define TOL 0.90

/* Con este algoritmo hallamos el rectangulo de la ventana de busqueda de
 * correlaciones para un pixel, de tal forma que no nos salgamos de la imagen
 */
void
limites_de_ventana (int i, int j, int w, int h,
                    int *x, int *y, int *wv, int* hv)
{
  *x = i - (BUSQUEDA / 2);
  *y = j - (BUSQUEDA / 2);
  
  if (*x < (BUSQUEDA / 2))
    *x = BUSQUEDA / 2;
  if (*y < (BUSQUEDA / 2))
    *y = BUSQUEDA / 2;
    
  *wv = BUSQUEDA; /* Longitud de la ventana*/
  *hv = BUSQUEDA; /* Altura de la ventana */
  
  if ( *wv + BUSQUEDA > (w-1) - (VENTANA/2)) /* Si nos pasamos del limite por la derecha limitamos la ventana*/
    *wv = (w-1) - BUSQUEDA/2;
  if ( *hv + BUSQUEDA > (h-1) - (VENTANA/2)) /* Si nos pasamos del limite por inferior limitamos la ventana */
    *hv = (h-1) - VENTANA/2;
}


float*
laplaciano (float *input, int width, int height)
{

	float  *output;
	float **lap;

	output = (float*)malloc (sizeof(float) * width * height);

	ami_malloc2d (lap, float, 3, 3);

	lap[0][0] = 1.0; lap[0][1] =  1.0; lap[0][2] = 1.0;
	lap[1][0] = 1.0; lap[1][1] = -8.0; lap[1][2] = 1.0;
	lap[2][0] = 1.0; lap[2][1] =  1.0; lap[2][2] = 1.0;

	aan_mascara_canal (input, output, width, height, lap);
  ami_free2d (lap);

	return output;
}

void
normalizar (float *input, int length)
{
  int i;
  float max = input[0];
  float min = input[0];
  
  for (i=0; i<length; i++)
  {
    if (input[0] > max)
      max = input[0];
    if (input[0] > min)
      min = input[0];
  }
  
  for (i=0; i<length; i++)
  {
    float tmp;
    
    tmp = input[0] - min;
    input[0] = tmp / max;
    
    input[0] = 0.5 + input[0] - 0.5;
  }
}

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
  int xb, yb, wb, hb;
  float ac, ab;
  float *area   = (float*)malloc (sizeof (float) * VENTANA * VENTANA);
  
  float media_ventana = media (vent, VENTANA * VENTANA);
  float stddev_ventana = stddev (vent, VENTANA * VENTANA, media_ventana);

  /* Hallamos el rectangulo del area de busqueda */
  limites_de_ventana (vent_i, vent_j, width, height, &xb, &yb, &wb, &hb);

  for (i = xb; i < xb + wb; i++)
  {
    for (j = yb; j < yb + hb; j++)
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

      /* Calculamos las distancias para compararlas */
      ab = sqrtf ((float) ((vent_i - i)*(vent_i - i) + (vent_j - j)*(vent_j - j)));
      ac = sqrtf ((float) ((*x)*(*x) + (*y)*(*y)));

      if (ab == 0) /* Si el pixel coincide con su original, descartamos este pixel */
      {
        *x = -1;
        *y = -1;
        free (area);
        return;
      }

      if (sum == corr)
      {
        /* Si la distancia de este pixel es mayor que */
        if (ac < ab)
          continue;
      }      
      else if (sum < corr)
        continue;

      corr = sum;
      *x = vent_i - i;
      *y = vent_j - j;      
    }
  }
  
  free (area);
}

void
aan_correlacion (float *a, float *b, int width, int height, float *horizontal, float *vertical)
{
  int i, j;
  float *area   = (float*)malloc (sizeof (float) * VENTANA * VENTANA);
  float *lap;

  for (i=0; i < width * height; i++)
  {
    /* Ponemos todos los valores de la salida a 0 */
    vertical[i] = 0.0;
    horizontal[i] = 0.0;
  }
  
  /* Laplaciano */
  lap = laplaciano (a, width, height);
  
  /* Ignoramos los pixeles de los bordes por no poder llenar una ventana */
  for (i = VENTANA / 2; i < width - (VENTANA / 2); i++)
  {
    for (j = VENTANA / 2; j < height - (VENTANA / 2); j++)
    {
      int x = -1, y = -1;

     if (lap[width*j + i] < 0.5)
        continue;

      copiar_ventana (a, area, width, height, i, j);
      buscar_correlacion (area, b, width, height, i, j, &x, &y);

      /* Si no hallamos correlacion continuamos */
      if (x == -1 || y == -1)
        continue;
      /* Si la correlacion nos devuelve el pixel original lo descartamos */
      if (x == i && y == j)
        continue;

      vertical[j*width + i] = x;
      horizontal[j*width + i] = y;
    }
  }

  normalizar (vertical,   width * height);
  normalizar (horizontal, width * height);

  free (lap);
  free (area);
}
