#ifndef __AAN_ONDAS_H__
#define __AAN_ONDAS_H__
void
aan_ecuacion_ondas_metodo_explicito(float *red_input,
                                    float *green_input,
                                    float *blue_input,
                                    float *red_output,
                                    float *green_output,
                                    float *blue_output,
                                    int    width,
                                    int    height,
                                    float dt,
                                    int Niter);
#endif
