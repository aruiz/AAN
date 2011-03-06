#include <stdio.h>
#include "lotka_volterra.h"

#define NITER 100

int
main (int argc, char** argv)
{
	float Presa, Cazador;

	float P0 = 0.1, C0 = 1.0;
	
	float a = 0.1;
	float b = 0.3;
	float d = 0.3;
	float e = 0.3;
	float f = 0.3;

	fprintf (stderr, "3.2: dt = 0");
	
	lotka_volterra (a, b, d, e, f,
	                P0, C0,
	                &Presa, &Cazador,
	                0.3,
	                NITER);

	return 0;
}
