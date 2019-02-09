/* See LICENSE file for copyright and license details */
#ifndef CPSO_H
#define CPSO_H

#include <stdlib.h>

/* Function pointer types */
typedef double (cpso_rng)(size_t *seed);
typedef double (cpso_fitness)(void *arg, size_t n, double *v);

struct cpso_result {
    double fx; /* Score the following vector achieved */
    double *x; /* Vector of values that minimized f */
};

struct cpso_config {
    size_t dim;   /* Number of dimensions to explore */
    size_t size;  /* Number of particles in the swarm */
    double b_lo; /* Lower bound of starting positions */
    double b_hi; /* Upper bound of starting positions */

    double cv; /* Previous velocity coefficient */
    double cl; /* Local best coefficient */
    double cg; /* Global best coefficient */

    size_t *seed;       /* Seed for the following rng */
    cpso_rng *rng; /* Uniform rng in the range of [0, 1] */

    void *arg;      /* Fitness function arg */
    cpso_fitness *f; /* Fitness function */
};

/* cpso_size() -  Compute the amount of memory needed for a particle swarm.
 * @swarm: Configuration used to compute the return value.
 *
 * Since cpso allows a particle swarm simulation to be run in steps, it
 * requires a buffer to store state. This buffer is provided by the user
 * to the functions cpso_init and cpso_run. The buffer may be either statically
 * or dynamically allocated, and its size is dependent on the cpso_config
 * structure passed to it.
 *
 * Returns the amount of bytes needed to store state using @swarm.
 */
size_t
cpso_size(struct cpso_config const *swarm);

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
cpso_init(struct cpso_result *g, void *buffer, struct cpso_config const *swarm);

/* cpso_step() - Simulate one round of particle updates in the swarm.
 * @g: Used to store the resulting vector and score of the swarm
 * @buffer: Memory to store simulation state for cpso_step and cspo_run.
 * @swarm: The swarm configuration dictating the details of the simulation.
 *
 * Simulate a single step of particle updates, meaning each particle is updated
 * once, and the result is updated to match the resulting state of the swarm.
 */
void
cpso_step(struct cpso_result *g, void *buffer, struct cpso_config const *swarm);

/* cpso_step() - Run multiple steps of the simulation.
 * @g: Used to store the resulting vector and score of the swarm
 * @buffer: Memory to store simulation state for cpso_step and cspo_run.
 * @swarm: The swarm configuration dictating the details of the simulation.
 * @steps: The number of rounds of particle updates to simulate
 *
 * Simulate as many iterations as specified in @steps. Equivalent to running
 * cpso_step in a loop for @steps iterations.
 */
void
cpso_run(struct cpso_result *g, void *buffer, struct cpso_config const *swarm, size_t steps);

/* cpso_rngU01() - The default RNG provided by cpso.
 * @seed: Stateful seed that may be used and mutated by the RNG.
 *
 * Generate a number in the range [0.0, 1.0] in as uniform a distribution as
 * possible. Note, this RNG may not be perfectly evenly distributed, neither is
 * it secure. This RNG is meant to be fast and good enough for the particle
 * swarm simulation, not for applications requiring a cryptographically secure
 * RNG.
 *
 * Returns a double-precision float in the inclusive range [0.0, 1.0].
 */
double
cpso_rngU01(size_t *seed);

#endif
