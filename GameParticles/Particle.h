//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef PARTICLE_H
#define PARTICLE_H

// include
#include "Vect4D.h"
#include "Matrix.h"
#include "ParticleEmitter.h"

class Particle
{
public:
	friend class ParticleEmitter;
	
	Particle() = delete;	
	~Particle();
	Particle(const Particle& srcParticle);
	Particle& operator=(const Particle&) = delete;

	Particle(const ParticleEmitter* const pParticleEmitter);

	void Update(const float time_elapsed);

private:
	Vect4D	position;
	Vect4D	velocity;
	Matrix	scaleMatrix;

	float	life;
	float	rotation;
	uint32_t pad0;
	uint32_t pad1;

	const static Vect4D Z_AXIS;
	const static float ROTATION_VELOCITY;
};

#endif 

// --- End of File ---
