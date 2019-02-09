# CPSO - C Particle Swarm Optimization
This is a small library implementing a particle swarm optimization algorithm
in C. This library can either be installed and linked to, or simply be copied
into a project since it consists of only two files. The functions provided
can be used in a program to heuristically minimize a given function.

## Usage
First include the cpso header, configure the swarm, then call the cpso
initialization function. Then either cpso_step or cpso_run can be called
to run the particle swarm simulation.

## Swarm Configuration
See "cpso.h".

## Examples
See "demo.c".

## Installation
No external dependencies are required for this library other than a C89 compliant libc, make, compiler.
``` bash
make clean all
sudo make install
```
