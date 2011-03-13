all: 4_calor_explicito_p1 4_calor_explicito_p2 4_calor_explicito_p3 4_calor_explicito_p4 4_calor_explicito_negativo

4_calor_explicito_p1: 4_calor_explicito.c aan_calor_explicito.c aan_calor_explicito.h float_utils.c float_utils.h
	gcc -DP41 -Wall -lm -g 4_calor_explicito.c aan_calor_explicito.c float_utils.c -o 4_calor_explicito_p1
	
4_calor_explicito_p2: 4_calor_explicito.c aan_calor_explicito.c aan_calor_explicito.h float_utils.c float_utils.h
	gcc -DP42 -Wall -lm -g 4_calor_explicito.c aan_calor_explicito.c float_utils.c -o 4_calor_explicito_p2

4_calor_explicito_p3: 4_calor_explicito.c aan_calor_explicito.c aan_calor_explicito.h float_utils.c float_utils.h
	gcc -DP43 -Wall -lm -g 4_calor_explicito.c aan_calor_explicito.c float_utils.c -o 4_calor_explicito_p3

4_calor_explicito_p4: 4_calor_explicito.c aan_calor_explicito.c aan_calor_explicito.h float_utils.c float_utils.h
	gcc -DP44 -Wall -lm -g 4_calor_explicito.c aan_calor_explicito.c float_utils.c -o 4_calor_explicito_p4

4_calor_explicito_negativo: 4_calor_explicito.c aan_calor_explicito.c aan_calor_explicito.h float_utils.c float_utils.h
	gcc -DNEGATIVO -Wall -lm -g 4_calor_explicito.c aan_calor_explicito.c float_utils.c -o 4_calor_explicito_negativo

video: all
	cd p1 && make
	cd p2 && make
	cd p3 && make
	cd p4 && make
	cd negativo && make
clean:
	rm -rf 4_calor_explicito_negativo 4_calor_explicito_p4 4_calor_explicito_p3 4_calor_explicito_p2 4_calor_explicito_p1
	rm -rf */*.bmp */*.avi
