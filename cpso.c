/* See LICENSE file for copyright and license details */
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "cpso.h"

typedef struct {
	double *v;
	double *x;
	double *p;
} particle;

/* Allocate _size_ bytes from a buffer, and shift the buffer over */
static void *
balloc(void **buffer, size_t size)
{
	void *mem = *buffer;
	*(uint8_t **)buffer += size;
	return mem;
}

size_t
cpso_size(struct cpso_config const *cp)
{
	return cp->size * (sizeof(particle) + sizeof(double) * cp->dim * 3);
}

void
cpso_init(struct cpso_result *g, void *buffer, struct cpso_config const *swarm)
{
	size_t i, j;
	particle *p = buffer;
	double range = swarm->b_hi - swarm->b_lo;

	/* Move the buffer pointer over to start allocating the vectors */
	buffer = p + swarm->size;
	for (i = 0; i < swarm->size; ++i) {
		p[i].v = balloc(&buffer, sizeof(*p[i].v) * swarm->dim);
		p[i].x = balloc(&buffer, sizeof(*p[i].x) * swarm->dim);
		p[i].p = balloc(&buffer, sizeof(*p[i].p) * swarm->dim);
		for (j = 0; j < swarm->dim; ++j) {
			/* Initialize the vector randomly between the low and high */
			p[i].x[j] = range * swarm->rng(swarm->seed) + swarm->b_lo;
			p[i].p[j] = p[i].x[j];
			p[i].v[j] = 2 * range * swarm->rng(swarm->seed) - range;
		}
	}
	/* Initialize the result to a random point */
	for (j = 0; j < swarm->dim; ++j) {
		g->x[j] = (swarm->b_hi - swarm->b_lo)
				  * swarm->rng(swarm->seed)
				  + swarm->b_lo;
	}
}

void
cpso_step(struct cpso_result *g, void *buffer, struct cpso_config const *swarm)
{
	size_t i, j;
	double rl, rg;
	double pfx, pfp, gfx;
	particle *p = buffer;

	for (i = 0; i < swarm->size; ++i) {
		pfx = swarm->f(swarm->arg, swarm->dim, p[i].x);
		pfp = swarm->f(swarm->arg, swarm->dim, p[i].p);
		gfx = swarm->f(swarm->arg, swarm->dim, g->x);

		/* Compared new position to local best, and global best */
		if (pfx < pfp) {
			for (j = 0; j < swarm->dim; ++j)
				p[i].p[j] = p[i].x[j];
			if (pfp < gfx) {
				for (j = 0; j < swarm->dim; ++j)
					g->x[j] = p[i].p[j];
				g->fx = gfx;
			}
		}
	}

	for (i = 0; i < swarm->size; ++i) {
		/* Update the particle's velocity */
		for (j = 0; j < swarm->dim; ++j) {
			rl = swarm->rng(swarm->seed);
			rg = swarm->rng(swarm->seed);
			p[i].v[j] = swarm->cv * p[i].v[j]
			          + swarm->cl * rl * (p[i].p[j] - p[i].x[j])
			          + swarm->cg * rg * (g->x[j] - p[i].x[j]);
		}
		/* Update the particle's position */
		for (j = 0; j < swarm->dim; ++j)
			p[i].x[j] += p[i].v[j];
	}
}

void
cpso_run(struct cpso_result *g,
         void *buffer,
         struct cpso_config const *swarm,
         size_t steps)
{
	size_t i;

	for (i = 0; i < steps; ++i) {
		cpso_step(g, buffer, swarm);
	}
}

/* TODO(todd): Stop using rand, and implement an RNG */
double
cpso_rngU01(size_t *seed)
{ 
	/*
	unsigned short x;

	if (NULL == seed)
		return 0.0;
	x = *seed;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	*seed = x;
	return (double)x / (double)((1 << 16) - 1);
	*/
	return (double)rand() / (double)RAND_MAX;
}
