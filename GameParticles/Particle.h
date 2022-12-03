//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef PARTICLE_H
#define PARTICLE_H

// include
#include "Vect4D.h"
#include "Matrix.h"
#include "ParticleEmitter.h"

class Particle : public Align16
{
public:
	friend class ParticleEmitter;
	
	Particle() = delete;	
	~Particle();
	Particle(const Particle& srcParticle) = default;
	Particle& operator=(const Particle&) = delete;

	Particle(const ParticleEmitter* const pParticleEmitter);

	void Update(const float time_elapsed);

private:
	Vect4D	position;
	Vect4D	velocity;

	float	life;
	float	rotation;
	Vect4D	scaleVect;
	uint32_t pad1;

	const static Vect4D Z_AXIS;
	const static float ROTATION_VELOCITY;
};

#endif 

// --- End of File ---
