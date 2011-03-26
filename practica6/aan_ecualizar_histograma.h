#ifndef __AAN_ECUALIZAR_HISTOGRAMA_H__
#define __AAN_ECUALIZAR_HISTOGRAMA_H__

void
aan_ecualizar_histograma(float *h, float *e, int *f);

void
aan_ecualizar_histograma_canal(float *canal_input,
                               float *canal_output,
                               int width,
                               int height,
                               int *f);

#endif /* __AAN_ECUALIZAR_HISTOGRAMA_H__ */
