#ifndef __LOTKA_VOLTERRA_H__
#define __LOTKA_VOLTERRA_H__
void lotka_volterra (float  a, float b, float d, float e, float f,
                     float  P0, float C0,
                     float *Presa,
                     float *Cazador,
                     float  dt,
                     int    niter);
#endif
