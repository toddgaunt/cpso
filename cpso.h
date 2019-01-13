/* See LICENSE file for copyright and license details */
#ifndef CPSO_H
#define CPSO_H

#include <stdlib.h>

typedef struct {
	double fx; /* Score the following vector achieved */
	double *x; /* Vector of values that minimized f */
} cpso_result;

typedef struct {
	size_t dim;   /* Number of dimensions to explore */
	size_t size;  /* Number of particles in the swarm */
	double b_lo; /* Lower bound of starting positions */
	double b_hi; /* Upper bound of starting positions */

	double cv; /* Previous velocity coefficient */
	double cl; /* Local best coefficient */
	double cg; /* Global best coefficient */

	long *seed;                   /* Seed for the following rng */
	double (*rngU01)(long *seed); /* Uniform rng in the range of [0, 1] */

	void *arg;                                   /* Fitness function arg */
	double (*f)(void *arg, size_t n, double *v); /* Fitness function */
} cpso_config;

/* cpso_size() -  Compute the amount of memory needed for a particle swarm.
 * @swarm: Configuration used to compute the return value.
 *
 * Since cpso allows a particle swarm simulation to be run in steps, it
 * requires a buffer to store state. This buffer is provided by the user
 * to the functions cpso_init and cpso_run. The buffer may be either statically
 * or dynamically allocated, and its size may vary between different cpso
 * configurations.
 *
 * Returns the amount of bytes needed to store state using @swarm.
 */
size_t
cpso_size(cpso_config const *swarm);

/* cpso_init() - Initialize the result and state buffer. 
 * @g: Result buffer to initialize for the swarm simulation.
 * @buffer: Memory to initialize for the swarm simulation.
 * @swarm: The configuration for how @g and @buffer are initialized.
 *
 * This function initializes @g and @buffer with values using the configuration
 * provided by @swarm. This is necessary because cpso_step and cpso_run assume
 * that the memory in @g and @buffer is initialized properly, and will not work
 * properly if zero initialized or left uninitialized.
 */
void
cpso_init(cpso_result *g, void *buffer, cpso_config const *swarm);

/* cpso_step() - Simulate one round of particle updates in the swarm.
 * @g: Used to store the resulting vector and score of the swarm
 * @buffer: Memory to store simulation state for cpso_step and cspo_run.
 * @swarm: The swarm configuration dictating the details of the simulation.
 */
void
cpso_step(cpso_result *g, void *buffer, cpso_config const *swarm);

/* cpso_step() - Run multiple steps of the simulation.
 * @g: Used to store the resulting vector and score of the swarm
 * @buffer: Memory to store simulation state for cpso_step and cspo_run.
 * @swarm: The swarm configuration dictating the details of the simulation.
 * @steps: The number of rounds of particle updates to simulate
 */
void
cpso_run(cpso_result *g, void *buffer, cpso_config const *swarm, size_t steps);

double
cpso_rngU01(long *seed);

#endif
