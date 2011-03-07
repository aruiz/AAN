#include <stdio.h>
#include "lotka_volterra.h"

#define NITER 206100

int
main (int argc, char** argv)
{
	float Presa, Cazador;

	float P0 = 1000, C0 = 100;
	
	float a = 0.1;
	float b = 0.1;
	float d = 0.1;
	float e = 0.1;
	float f = 0.1;

	float dt = 0.001;

	fprintf (stderr, "3.2: dt = 0\n");
	
	lotka_volterra (a, b, d, e, f,
	                P0, C0,
	                &Presa, &Cazador,
	                dt,
	                NITER);

	return 0;
}
