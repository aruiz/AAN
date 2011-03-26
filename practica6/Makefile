all: 6_ecualizacion_histogramas

6_ecualizacion_histogramas: 6_ecualizacion_histogramas.c float_utils.c float_utils.h ami.h ami_bmp.h
	gcc -Wall -DP1 -lm -g 6_ecualizacion_histogramas.c float_utils.c -o 6_ecualizacion_histogramas

clean:
	rm -rf 6_ecualizacion_histogramas
	rm -r */*avi */*bmp
