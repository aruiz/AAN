#include <stdio.h>
#include "lotka_volterra.h"

int
main (int argc, char** argv)
{
	float Presa, Cazador;
	float P0 = 1.0, C0 = 0.5;
	float a = 100.3;
	float d = 0.3;

	lotka_volterra (a, 0, d, 0, 0,
	                P0, C0,
	                &Presa, &Cazador,
	                0.25,
	                1000);

	return 0;
}
