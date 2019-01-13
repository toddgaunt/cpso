/* See LICENSE file for copyright and license details */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cpso.h"

char const *argv0;
double rx;
double ry;

void
usage()
{
	fprintf(stderr, "usage: %s <number of particles> <number of iterations>\n", argv0);
	exit(EXIT_FAILURE);
}

double
distance_fitness(void *arg, size_t n, double *v)
{
	(void)arg;
	(void)n;

	return sqrt(pow(rx - v[0], 2) + pow(ry - v[1], 2));

}

int
main(int argc, char **argv)
{
	size_t n_particle;
	size_t n_iteration;

	long seed;
	double v[3];
	cpso_result g;
	void *buffer;
	cpso_config swarm;

	g.x = v;

	srand(time(NULL));
	argv0 = argv[0];
	if (argc != 3)
		usage();
	n_particle = strtol(argv[1], NULL, 10);
	n_iteration = strtol(argv[2], NULL, 10);

	/* Set the coordinates designated "most fit" within [-5000, 5000] */
	rx = 10000 * cpso_rngU01(NULL) - 5000;
	ry = 10000 * cpso_rngU01(NULL) - 5000;

	/* Set the configuration */
	swarm.dim = 2;
	swarm.size = n_particle;
	swarm.b_lo = -5000;
	swarm.b_hi = 5000;

	swarm.cv = 0.8;
	swarm.cl = 0.1;
	swarm.cg = 0.2;

	swarm.seed = &seed;
	swarm.rngU01 = cpso_rngU01;
	
	swarm.arg = NULL;
	swarm.f = distance_fitness;

	/* Allocate a buffer sized for the configuration */
	buffer = malloc(cpso_size(&swarm));

	cpso_init(&g, buffer, &swarm);
	cpso_run(&g, buffer, &swarm, n_iteration);

	free(buffer);
	printf("Target position: (%f, %f) = 0\n", rx, ry);
	printf("Swarm results:   (%f, %f) = %f\n", g.x[0], g.x[1], g.fx);
	return 0;
}
