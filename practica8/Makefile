all: 8_ecuacion_ondas_explicito

8_ecuacion_ondas_explicito: 8_ecuacion_ondas_explicito.c aan_ondas.c aan_ondas.h
	gcc -Wall -g 8_ecuacion_ondas_explicito.c  aan_ondas.c float_utils.c -o 8_ecuacion_ondas_explicito

video: 8_ecuacion_ondas_explicito
	cd p1 && make
	cd p2 && make

clean:
	rm -rf 8_ecuacion_ondas_explicito
	rm -rf */*avi */*bmp
