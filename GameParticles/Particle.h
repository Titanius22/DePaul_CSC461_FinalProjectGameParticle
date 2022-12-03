//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef PARTICLE_H
#define PARTICLE_H

// include
#include "Vect4D.h"
#include "Matrix.h"

class Particle
{
public:
	friend class ParticleEmitter;
	
	Particle() = delete;	
	~Particle();
	Particle(const Particle& srcParticle);
	Particle& operator=(const Particle&) = delete;

	Particle(const Vect4D& _position, const Vect4D& _velocity, const Vect4D& _scale);

	void Update(const float time_elapsed);

private:
	Particle *next;
	Particle *prev;

	float	life;
	float	rotation;
	Vect4D	position;
	Vect4D	velocity;
	Vect4D	scale;

	const static Vect4D Z_AXIS;
	const static float ROTATION_VELOCITY;
};

#endif 

// --- End of File ---
