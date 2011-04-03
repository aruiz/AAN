#ifndef __AAN_PROPAGACION_H__
#define __AAN_PROPAGACION_H__
void aan_ecuacion_propagacion_frentes(float *red_input,
                                      float *green_input,
                                      float *blue_input,
                                      float *red_output,
                                      float *green_output,
                                      float *blue_output,
                                      int width,
                                      int height,
                                      float dt,
                                      int Niter,
                                      float *F);
#endif
