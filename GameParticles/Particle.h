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
	
	Particle();	
	~Particle();
	Particle(const Particle& srcParticle);
	Particle& operator=(const Particle&) = delete;

	Particle(const float _life, const Vect4D& _position, const Vect4D& _velocity, const Vect4D& _scale);

	void Update(const float& time_elapsed);
	//void CopyDataOnly( Particle *p );
private:
	Particle *next;
	Particle *prev;

	float	life;
	float	rotation;
	Vect4D	position;
	Vect4D	velocity;
	Vect4D	scale;
	float	rotation_velocity;

	// dead space
	float	pad0;
	float	pad1;
	float	pad2;

	// matrices to hold data
	Matrix diff_matrix;
	Matrix prev_matrix;
	Matrix curr_matrix;

	const static Vect4D Z_AXIS;
};

#endif 

// --- End of File ---
