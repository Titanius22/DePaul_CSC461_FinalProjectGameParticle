//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#include "Particle.h"
#include "Matrix.h"

const Vect4D Particle::Z_AXIS(0.0f, 0.0f, -5.0f);

Particle::Particle()
	: next(nullptr), prev(nullptr), life(0.0f), rotation(0.0f),
	  position(Vect4D(0.0f, 0.0f, -6.0f)),
	  velocity(Vect4D(-1.0f, 0.0f, 0.0f)),
	  scale(Vect4D(1.0f, 1.0f, 1.0f)),
	  rotation_velocity(0.15f)
{}

Particle::Particle(const Particle& srcParticle)
	: next(nullptr), prev(nullptr), life(srcParticle.life), rotation(srcParticle.rotation),
	position(srcParticle.position),
	velocity(srcParticle.velocity),
	scale(srcParticle.scale),
	rotation_velocity(srcParticle.rotation_velocity)
{}

Particle::Particle(const float _life, const Vect4D& _position, const Vect4D& _velocity, const Vect4D& _scale)
	: next(nullptr), prev(nullptr), life(_life), rotation(0.0f),
	position(_position),
	velocity(_velocity),
	scale(_scale),
	rotation_velocity(0.15f)
{}

Particle::~Particle()
{
	// nothing to do
}

void Particle::Update(const float time_elapsed)
{
	// serious math below - magic secret sauce
	life += time_elapsed;
	position += (velocity * time_elapsed);
	Vect4D v;
	position.Cross(Z_AXIS, v);
	v.norm(v);
	position += (v * 0.05f * life);

	rotation += (rotation_velocity * time_elapsed) ;
}

// --- End of File ---
