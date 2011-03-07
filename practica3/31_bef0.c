#include <stdio.h>
#include "lotka_volterra.h"

#define NITER 100

int
main (int argc, char** argv)
{
	float Presa, Cazador;
	float P0 = 1000.0, C0 = 100.0;
	float a = 100.3;
	float d = 0.3;

	fprintf (stderr, "3.1: b = e = f = 0\n");
	lotka_volterra (a, 0, d, 0, 0,
	                P0, C0,
	                &Presa, &Cazador,
	                0.01,
	                NITER);
	return 0;
}
