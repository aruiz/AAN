#ifndef __AAN_MASCARA_H__
#define __AAN_MASCARA_H__

void
aan_mascara_canal (float  *canal_input,
                   float  *canal_output,
                     int   width,
                     int   height,
                   float **m);

void
aan_mascara_imagen (float *red_input,
                    float *green_input,
                    float *blue_input,
                    float *red_output,
                    float *green_output,
                    float *blue_output,
                    int width,
                    int height,
                    float **m);

#endif
