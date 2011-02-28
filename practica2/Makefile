all: 2_mascara_grad_horizontal 2_mascara_grad_vertical 2_mascara_laplaciana 2_mascara_mod_gradiente 2_mascara_byn

2_mascara_laplaciana: 2_mascara_laplaciana.c \
                      float_utils.c float_utils.h \
                      aan_normalizar.c aan_normalizar.h \
                      aan_mascara.c aan_mascara.h \
                      aan_unir_canales.c aan_unir_canales.h \
                      ami.h ami_bmp.h
	gcc -Wall -lm -g 2_mascara_laplaciana.c float_utils.c aan_normalizar.c aan_mascara.c aan_unir_canales.c -o 2_mascara_laplaciana
	
2_mascara_grad_horizontal: 2_mascara_grad_horizontal.c \
                           float_utils.c float_utils.h \
                           aan_normalizar.c aan_normalizar.h \
                           aan_mascara.c aan_mascara.h \
                           aan_unir_canales.c aan_unir_canales.h \
                           ami.h ami_bmp.h
	gcc -Wall -lm -g 2_mascara_grad_horizontal.c float_utils.c aan_normalizar.c aan_mascara.c aan_unir_canales.c -o 2_mascara_grad_horizontal

2_mascara_grad_vertical: 2_mascara_grad_vertical.c \
                           float_utils.c float_utils.h \
                           aan_normalizar.c aan_normalizar.h \
                           aan_mascara.c aan_mascara.h \
                           aan_unir_canales.c aan_unir_canales.h \
                           ami.h ami_bmp.h
	gcc -Wall -lm -g 2_mascara_grad_vertical.c float_utils.c aan_normalizar.c aan_mascara.c aan_unir_canales.c -o 2_mascara_grad_vertical

2_mascara_mod_gradiente: 2_mascara_mod_gradiente.c \
                         float_utils.c float_utils.h \
                         aan_normalizar.c aan_normalizar.h \
                         aan_mascara.c aan_mascara.h \
                         aan_unir_canales.c aan_unir_canales.h \
                         ami.h ami_bmp.h
	gcc -Wall -lm -g 2_mascara_mod_gradiente.c float_utils.c aan_normalizar.c aan_mascara.c aan_unir_canales.c -o 2_mascara_mod_gradiente

2_mascara_byn: 2_mascara_byn.c \
               float_utils.c float_utils.h \
               aan_normalizar.c aan_normalizar.h \
               aan_mascara.c aan_mascara.h \
               aan_unir_canales.c aan_unir_canales.h \
               ami.h ami_bmp.h
	gcc -Wall -lm -g 2_mascara_byn.c float_utils.c aan_normalizar.c aan_mascara.c aan_unir_canales.c -o 2_mascara_byn

test:
	./2_mascara_laplaciana ../practica1/img/pp1.bmp
	./2_mascara_grad_horizontal ../practica1/img/pp1.bmp
	./2_mascara_grad_vertical ../practica1/img/pp1.bmp
	./2_mascara_mod_gradiente ../practica1/img/pp1.bmp
	./2_mascara_byn  img/pp1_byn.bmp

clean:
	rm -rf *bmp 2_mascara_laplaciana 2_mascara_grad_vertical 2_mascara_grad_horizontal 2_mascara_mod_gradiente 2_mascara_byn
