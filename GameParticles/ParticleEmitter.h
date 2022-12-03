//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Matrix.h"
#include "Vect4D.h"
#include "Particle.h"

#include <list>

class ParticleEmitter
{
public:
	friend class Particle;

	ParticleEmitter();
	~ParticleEmitter();
	ParticleEmitter(const ParticleEmitter&) = delete;
	ParticleEmitter& operator=(const ParticleEmitter&) = delete;
	
	void SpawnParticle();
	void update();
	void draw() const;

	void addParticleToList(Particle *p );
	void removeParticleFromList( Particle *p );

	const static Matrix TRANS_CAMERA_MATRIX;
	const static Matrix INVERSE_TRANS_CAMERA_MATRIX;
	const static Matrix TRANS_INVERSE_TRANS_CAMERA_MATRIX;

private:
	const Vect4D	start_position;
	const Vect4D	start_velocity;
	const Vect4D	start_scale;

	const float	spawn_frequency;
	float	last_spawn;
	float	last_loop;	
	int		last_active_particle;

	const Vect4D	vel_variance;
	const Vect4D	pos_variance;
	const float	scale_variance;

	// List data
	Particle* const pParticleBlockStart;
	Particle* const pParticleBlockEnd;
	Particle* headParticle;
	Particle* tailParticle;
};

#endif 

// --- End of File ---
