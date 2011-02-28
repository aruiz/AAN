#ifndef __AAN_UNIR_CANALES_H__
#define __AAN_UNIR_CANALES_H__

#include <stdlib.h>

/* Funciones para unir dos canales en uno solo con una separacion de
 * cuatro pixeles entre ellos. Ambos generan un nuevo canal cuya memoria
 * debe ser liberada una vez los datos hayan sido utilizados.
 */

/* Implementacion para precision entera (valores entre 0 y 255) */
void
aan_unir_canales_unsigned_char (unsigned char  *canal1,
                                unsigned char  *canal2,
                                unsigned char **canal_output,
                                          int   width,
                                          int   height);

/* Implementacion para precision flotante (valores entre 0.0 y 0.1) */
void
aan_unir_canales_float (float  *canal1,
                        float  *canal2,
                        float **canal_output,
                        int   width,
                        int   height);
#endif
