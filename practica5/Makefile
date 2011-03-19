all: 5_calor_explicito_fuerza_externa_p1 5_calor_explicito_fuerza_externa_p2

5_calor_explicito_fuerza_externa_p1: 5_calor_explicito_fuerza_externa.c aan_calor_explicito_fuerza_externa.c aan_calor_explicito_fuerza_externa.h float_utils.c float_utils.h
	gcc -Wall -DP1 -lm -g 5_calor_explicito_fuerza_externa.c aan_calor_explicito_fuerza_externa.c float_utils.c -o 5_calor_explicito_fuerza_externa_p1

5_calor_explicito_fuerza_externa_p2: 5_calor_explicito_fuerza_externa.c aan_calor_explicito_fuerza_externa.c aan_calor_explicito_fuerza_externa.h float_utils.c float_utils.h
	gcc -Wall -DP2 -lm -g 5_calor_explicito_fuerza_externa.c aan_calor_explicito_fuerza_externa.c float_utils.c -o 5_calor_explicito_fuerza_externa_p2

video: 5_calor_explicito_fuerza_externa_p1 5_calor_explicito_fuerza_externa_p2
	cd p1 && make
	cd p2 && make

clean:
	rm -rf 5_calor_explicito_fuerza_externa_p1 5_calor_explicito_fuerza_externa_p2
	rm -r */*avi */*bmp
