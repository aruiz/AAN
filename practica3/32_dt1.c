#include <stdio.h>
#include "lotka_volterra.h"

#define NITER 100000

int
main (int argc, char** argv)
{
	float Presa, Cazador;

	float P0 = 1000.0, C0 = 100.0;
	
	float a = 0.2;
	float b = 0.00015;
	float d = 0.0001;
	float e = 0.1;
	float f = 0.00015;


	fprintf (stderr, "3.2: dt = 0\n");
	
	lotka_volterra (a, b, d, e, f,
	                P0, C0,
	                &Presa, &Cazador,
	                1.0,
	                NITER);

	return 0;
}
