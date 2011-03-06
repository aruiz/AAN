#include <stdio.h>

void
lotka_volterra (float  a,  float b, float d, float e, float f,
                float  P0, float C0,
                float *Presa,
                float *Cazador,
                float  dt,
                int    niter)
{
	int i;
	float dP, dC;
	
	/*TODO: Comprobar valores */
	
	*Presa = P0;
	*Cazador = C0;

	printf ("Instante\tCazador\tPresa\n");
	
	for (i=0; i<niter; i++)
	{
		dP = dt*a*(*Presa) - dt*b*(*Presa)*(*Cazador) - dt*d*(*Presa)*(*Presa);
		dC = - dt*e*(*Cazador) + dt*f*(*Presa)*(*Cazador);
		
		*Presa = (*Presa) + dP;
		*Cazador = (*Cazador) + dC;
		
		printf ("%f\t%f\t%f\n", dt * (float)i, *Cazador, *Presa);
	}
}	
