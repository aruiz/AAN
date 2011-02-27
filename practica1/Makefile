all: 11_union_canales 12_normalizar

11_union_canales: 11_union_canales.c aan_unir_canales.c aan_unir_canales.h ami_bmp.h ami.h
	gcc -Wall -g 11_union_canales.c aan_unir_canales.c -o 11_union_canales
	
12_normalizar: 12_normalizar.c aan_unir_canales.c aan_normalizar.c	ami_bmp.h ami.h
	gcc -Wall -g 12_normalizar.c aan_unir_canales.c aan_normalizar.c -o 12_normalizar
	
clean:
	rm -rf *bmp
	rm 11_union_canales
