all: 10_correlacion

10_correlacion: 10_correlacion.c float_utils.c float_utils.h aan_correlacion.c aan_correlacion.h ami.h ami_bmp.h aan_mascara.c aan_mascara.h
	gcc -lm -o 10_correlacion -g 10_correlacion.c float_utils.c aan_correlacion.c aan_mascara.c

clean:
	rm -rf 10_correlacion
