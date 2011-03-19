#ifndef __AAN_CALOR_EXPLICITO_FUERZA_EXTERNA_H__
#define __AAN_CALOR_EXPLICITO_FUERZA_EXTERNA_H__
void
aan_ecuacion_calor_fuerza_externa_metodo_explicito (float  *canal_input,
                                                    float  *canal_output,
                                                    int     width,
                                                    int     height,
                                                    float   dt,
                                                    int     niter,
                                                    float   a,
                                                    float   t0,
                                                    float   t1,
                                                    float   t2);
#endif /* __AAN_CALOR_EXPLICITO_FUERZA_EXTERNA_H__ */
