all: 31_bef0 32_dt1 33_equilibrio 34_extincion

31_bef0: lotka_volterra.h lotka_volterra.c 31_bef0.c
	gcc -g 31_bef0.c lotka_volterra.c -o 31_bef0
	
32_dt1: 32_dt1.c lotka_volterra.h lotka_volterra.c
	gcc -g 32_dt1.c lotka_volterra.c -o 32_dt1
	
33_equilibrio: 33_equilibrio.c lotka_volterra.h lotka_volterra.c
	gcc -g 33_equilibrio.c lotka_volterra.c -o 33_equilibrio

34_extincion: 34_extincion.c lotka_volterra.h lotka_volterra.c
	gcc -g 34_extincion.c lotka_volterra.c -o 34_extincion
	
test: 31_bef0 32_dt1 33_equilibrio 34_extincion
	./31_bef0 > 31_bef0.csv
	./32_dt1 > 32_dt1.csv
	./33_equilibrio > 33_equilibrio.csv
	./34_extincion > 34_extincion.csv
	
	
