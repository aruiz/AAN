all: 7_propagacion_p1 7_propagacion_p2

7_propagacion_p1: 7_propagacion_p1.c aan_propagacion.c aan_propagacion.h
	gcc -Wall -DP1 -lm -g 7_propagacion_p1.c aan_propagacion.c aan_mascara.c float_utils.c -o 7_propagacion_p1

7_propagacion_p2: 7_propagacion_p2.c aan_propagacion.c aan_propagacion.h
	gcc -Wall -DP1 -lm -g 7_propagacion_p2.c aan_propagacion.c aan_mascara.c float_utils.c -o 7_propagacion_p2

video: 5_calor_explicito_fuerza_externa_p1 5_calor_explicito_fuerza_externa_p2
	cd p1 && make
	cd p2 && make

clean:
	rm -rf 7_propagacion_p1
	rm -r */*avi */*bmp
