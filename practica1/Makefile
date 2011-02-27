all: 11_union_canales 12_normalizar 13_rgb_to_hsv

11_union_canales: 11_union_canales.c \
                  aan_unir_canales.c aan_unir_canales.h \
                  float_utils.c float_utils.h \
                  ami_bmp.h ami.h
	gcc -Wall -g 11_union_canales.c aan_unir_canales.c float_utils.c -o 11_union_canales
	
12_normalizar: 12_normalizar.c \
               aan_normalizar.c aan_normalizar.h \
               aan_unir_canales.c aan_unir_canales.h \
               float_utils.c float_utils.h \
               ami_bmp.h ami.h
	gcc -Wall -g 12_normalizar.c aan_unir_canales.c aan_normalizar.c float_utils.c -o 12_normalizar

13_rgb_to_hsv: 13_rgb_to_hsv.c \
               aan_unir_canales.c aan_unir_canales.h \
               ami_bmp.h ami.h
	gcc -Wall -g 13_rgb_to_hsv.c aan_unir_canales.c -o 13_rgb_to_hsv
	
clean:
	rm -rf *bmp
	rm 11_union_canales 12_normalizar rgb_to_hsv
