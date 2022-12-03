//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef PARTICLE_H
#define PARTICLE_H

// include
#include "Vect4D.h"

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

	Vect4D  diff_Row0;
	Vect4D  diff_Row1;
	Vect4D  diff_Row2;
	Vect4D  diff_Row3;

	Vect4D	prev_Row0;
	Vect4D	prev_Row1;
	Vect4D  prev_Row2;
	Vect4D  prev_Row3;
	
	Vect4D	curr_Row0;
	Vect4D	curr_Row1;
	Vect4D  curr_Row2;
	Vect4D  curr_Row3;
};

#endif 

// --- End of File ---
