all: 6_ecualizacion_histogramas

6_ecualizacion_histogramas: 6_ecualizacion_histogramas.c aan_ecualizar_histograma.c aan_unir_canales.c aan_unir_canales.h aan_ecualizar_histograma.h float_utils.c float_utils.h ami.h ami_bmp.h
	gcc -Wall -DP1 -lm -g 6_ecualizacion_histogramas.c float_utils.c aan_unir_canales.c aan_ecualizar_histograma.c -o 6_ecualizacion_histogramas

clean:
	rm -rf 6_ecualizacion_histogramas
	rm -rf imagen_ecualizada_e*.bmp
