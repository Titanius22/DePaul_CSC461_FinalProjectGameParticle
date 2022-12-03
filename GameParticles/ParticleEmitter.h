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
	ParticleEmitter();
	~ParticleEmitter();
	ParticleEmitter(const ParticleEmitter&) = delete;
	ParticleEmitter& operator=(const ParticleEmitter&) = delete;
	
	void SpawnParticle();
	void update();
	void draw();

	void addParticleToList(Particle *p );
	void removeParticleFromList( Particle *p );

	void Execute(Particle* srcParticle);

	const static Matrix IDENT_MATRIX;
	const static Matrix TRANS_CAMERA_MATRIX;

private:
	const Vect4D	start_position;
	const Vect4D	start_velocity;
	const Vect4D	start_scale;

	const float	spawn_frequency;
	float	last_spawn;
	float	last_loop;	
	const float	max_life;
	const int		max_particles;
	int		last_active_particle;

	// added for speed efficiency
	int bufferCount;
	Particle *headParticle;
	const Vect4D	vel_variance;
	const Vect4D	pos_variance;
	float	scale_variance;
	std::list<Particle> drawBuffer;
	std::list<Particle> particle_list;
};

#endif 

// --- End of File ---
