# CPSO - C Particle Swarm Optimization
This is a small library implementing a particle swarm optimization algorithm
in C.

## Usage
First include the cpso header, configure the swarm, then call the cpso
initialization function. Then either cpso_step or cpso_run can be called
to run the particle swarm simulation. For an example, see "demo.c".

## Installation
No external dependencies are required for this library other than a C89 compliant libc, make, compiler.
``` bash
make clean all
sudo make install
```
