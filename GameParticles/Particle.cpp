//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#include "Particle.h"

const Vect4D Particle::Z_AXIS(0.0f, 0.0f, -5.0f);
const float Particle::ROTATION_VELOCITY(0.15f);

//Particle::Particle()
//	: next(nullptr), prev(nullptr), life(0.0f), rotation(0.0f),
//	  position(Vect4D(0.0f, 0.0f, -6.0f)),
//	  velocity(Vect4D(-1.0f, 0.0f, 0.0f)),
//	  scale(Vect4D(1.0f, 1.0f, 1.0f)),
//	  rotation_velocity(0.15f)
//{}

Particle::Particle(const Particle& srcParticle)
	: next(nullptr), prev(nullptr), life(srcParticle.life), rotation(srcParticle.rotation),
	position(srcParticle.position),
	velocity(srcParticle.velocity),
	scale(srcParticle.scale)
{}

Particle::Particle(const Vect4D& _position, const Vect4D& _velocity, const Vect4D& _scale)
	: next(nullptr), prev(nullptr), life(0.0f), rotation(0.0f),
	position(_position),
	velocity(_velocity),
	scale(_scale)
{}

Particle::Particle(const ParticleEmitter* const pParticleEmitter)
	: next(nullptr), prev(nullptr), life(0.0f), rotation(0.0f),
	position(pParticleEmitter->start_position),
	velocity(pParticleEmitter->start_velocity),
	scale(pParticleEmitter->start_scale)
{
	// x - variance
	float var = static_cast<float>(rand() % 1000);
	int sign = rand() % 2;
	float t_var = pParticleEmitter->pos_variance.x;
	if (sign == 0)
	{
		// negative var
		this->position.x -= t_var * var;
	}
	else
	{
		// positive var
		this->position.x += t_var * var;
	}

	// y - variance
	var = static_cast<float>(rand() % 1000);
	sign = rand() % 2;
	t_var = pParticleEmitter->pos_variance.y;
	if (sign == 0)
	{
		// negative var
		this->position.y -= t_var * var;
	}
	else
	{
		// positive var
		this->position.y += t_var * var;
	}

	// z - variance
	var = static_cast<float>(rand() % 1000);
	sign = rand() % 2;
	t_var = pParticleEmitter->pos_variance.z;
	if (sign == 0)
	{
		// negative var
		this->position.z -= t_var * var;
	}
	else
	{
		// positive var
		this->position.z += t_var * var;
	}



	// x  - add velocity
	var = static_cast<float>(rand() % 1000);
	sign = rand() % 2;
	t_var = pParticleEmitter->vel_variance.x;
	if (sign == 0)
	{
		// negative var
		this->velocity.x -= t_var * var;
	}
	else
	{
		// positive var
		this->velocity.x += t_var * var;
	}

	// y - add velocity
	var = static_cast<float>(rand() % 1000);
	sign = rand() % 2;
	t_var = pParticleEmitter->vel_variance.y;
	if (sign == 0)
	{
		// negative var
		this->velocity.y -= var * (t_var * 3.0f);
	}
	else
	{
		// positive var
		this->velocity.y += t_var * var;
	}

	// z - add velocity
	var = static_cast<float>(rand() % 1000);
	sign = static_cast<int>(rand() % 2);
	t_var = pParticleEmitter->vel_variance.z;
	if (sign == 0)
	{
		// negative var
		this->velocity.z -= var * (t_var * 3.0f);
	}
	else
	{
		// positive var
		this->velocity.z += t_var * var;
	}


	// correct the sign
	var = static_cast<float>(rand() % 1000);
	sign = rand() % 2;

	if (sign == 0)
	{
		this->scale *= (var * (1.20f * 0.001f * -3.0f));
	}
	else
	{
		this->scale *= (var * (1.20f * 0.001f));
	}
}

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

	rotation += (ROTATION_VELOCITY * time_elapsed) ;
}

// --- End of File ---
