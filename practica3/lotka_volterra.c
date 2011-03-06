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
	if (a < 0.0 || b < 0.0 || d < 0.0 || e < 0.0 || f < 0.0 ||
	    P0 < 0.0 || C0 < 0.0 ||
	    dt < 0.0 ||
	    niter < 0)
	{
		fprintf (stderr, "No se admiten valores negativos para ninguno de los argumentos.\n");
		return;
	}

	if (!Presa || !Cazador)
	{
		fprintf (stderr, "No se admiten punteros nulos para almacenar los resultados.\n");
		return;
	}
	
	*Presa = P0;
	*Cazador = C0;
	
	printf ("Instante\tCazador\tPresa\n");
	
	for (i=0; i<niter; i++)
	{
		dP = dt*a*(*Presa) - dt*b*(*Presa)*(*Cazador) - dt*d*(*Presa)*(*Presa);
		dC = - dt*e*(*Cazador) + dt*f*(*Presa)*(*Cazador);
		
		*Presa = (*Presa) + dP;
		*Cazador = (*Cazador) + dC;
		
		/* No puede haber menos de 0 individuos */
		if ((*Presa) < 0.0)
			*Presa = 0.0;
		if ((*Cazador) < 0.0)
			*Cazador = 0.0;
		
		printf ("%f\t%f\t%f\n", dt * (float)i, *Cazador, *Presa);
	}
}	
