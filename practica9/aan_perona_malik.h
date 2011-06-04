#ifndef __AAN_PERONA_MALIK_H__
#define __AAN_PERONA_MALIK_H__

void
aan_perona_malik (float *red_input,
                  float *green_input,
                  float *blue_input,
                  float *red_output,
                  float *green_output,
                  float *blue_output,
                  float  dt,
                  float  dh,
                  float  lambda,
                  int    Niters);


#endif // __AAN_PERONA_MALIK_H__
