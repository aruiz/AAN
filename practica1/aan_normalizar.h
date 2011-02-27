#ifndef __AAN_NORMALIZAR_H__
#define __AAN_NORMALIZAR_H__

void
aan_normalizar_canal_unsigned_char (unsigned char *canal_input,
                                    unsigned char *canal_output,
                                    int width,
                                    int height);
                                    

void
aan_normalizar_imagen_unsigned_char (unsigned char *red,
                                     unsigned char *green,
                                     unsigned char *blue,
                                               int  width,
                                               int height);
#endif
