//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#include "OpenGLDevice.h"
#include "ParticleEmitter.h"
#include "Settings.h"

PerformanceTimer globalTimer;

ParticleEmitter::ParticleEmitter()
:	start_position( -1.0f, -4.0f, 1.0f),
	start_velocity( -2.0f, 3.0f, -0.10f),
	start_scale(-1.0f, -1.0f, -1.0f, 1.0f),
	spawn_frequency(0.00001f),		
	last_spawn(globalTimer.GetGlobalTime()),		
	last_loop(globalTimer.GetGlobalTime()),
	last_active_particle(-1),
	vel_variance(-29.0f * 0.0001f, 0.70f * 0.0001f, -1.0f * 0.001f),
	pos_variance(-3.50f * 0.0001f, 3.50f * 0.0001f, 5.0f * 0.001f),
	scale_variance(3.0f),
	pParticleBlockStart(new Particle[NUM_PARTICLES]),
	pParticleBlockEnd(pParticleBlockStart + NUM_PARTICLES),
	headParticle(pParticleBlockStart),
	tailParticle(headParticle)
{}

ParticleEmitter::~ParticleEmitter()
{
	delete[] pParticleBlockStart;
}

void ParticleEmitter::SpawnParticle()
{
	// if tail is end, start from the beginning
	if (tailParticle == pParticleBlockEnd)
	{
		tailParticle = pParticleBlockStart;
	}
	
	AZUL_PLACEMENT_NEW_BEGIN // < ---- - Add 1 / 3 --------------
	#undef new //<----- Add 2/3 --------------
	
	// create AND initialize new particle
	new(tailParticle) Particle(this);

	AZUL_PLACEMENT_NEW_END //< ---- - Add 3 / 3 --------------

	tailParticle++;

	// increment count
	last_active_particle++;
}

void ParticleEmitter::update()
{
	// get current time
	float current_time = globalTimer.GetGlobalTime();

	// spawn particles
	float time_elapsed = current_time - last_spawn;
	
	// check if particles need to get added.....
	if (last_active_particle < NUM_PARTICLES - 1)
	{
		// add particles while while there are more to add
		while ((spawn_frequency < time_elapsed) && (last_active_particle < (NUM_PARTICLES - 1)))
		{
			// spawn a particle
			this->SpawnParticle();
			// adjust time
			time_elapsed -= spawn_frequency;
			// last time
			last_spawn = current_time;
		}
	}
	
	// total elapsed
	time_elapsed = current_time - last_loop;

	Particle* p = this->headParticle;
	// walk the particles

	do
	{
		// placing this at the beginning allows p==pParticleBlockEnd to go 
		//     into the while where tailParticle==pParticleBlockEnd might be true
		if (p == pParticleBlockEnd)
		{
			p = pParticleBlockStart;
		}
		
		// call every particle and update its position 
		p->Update(time_elapsed);

		// if life is greater that the max_life 
		// and there is some left on the list
		// remove node
		if((p->life > MAX_LIFE) && (last_active_particle > 0))
		{
			assert(p == headParticle);

			headParticle++;
			if (headParticle == pParticleBlockEnd)
			{
				headParticle = pParticleBlockStart;
			}

			// update the number of particles
			last_active_particle--;
		}

		p++;

		assert(p != nullptr);
	} while (p != tailParticle);

	// make sure the counts track (asserts go away in release - relax Christos)
	last_loop = current_time;
}

void ParticleEmitter::draw() const
{
	Matrix tmp(Matrix::IDENTITY_MATRIX);
	__m128 m128Tmp;
	float cosVar;
	float sinVar;

	// iterate throught the list of particles
	Particle* p = headParticle;
	do
	{
		assert(p != nullptr);

		// placing this at the beginning allows p==pParticleBlockEnd to go 
		//     into the while where tailParticle==pParticleBlockEnd might be true
		if (p == pParticleBlockEnd)
		{
			p = pParticleBlockStart;
		}
		
		// total transformation of particle
		///////////////////////////////////////////////////////////////////////////////
		cosVar = cos(p->rotation);
		sinVar = sin(p->rotation);
		m128Tmp = _mm_set_ps(cosVar, -sinVar, sinVar, cosVar);

		tmp.v0_m128.m128_u64[0] = m128Tmp.m128_u64[0];
		tmp.v1_m128.m128_u64[0] = m128Tmp.m128_u64[1];
		
		tmp.v3_m128 =
			_mm_hadd_ps(
				_mm_mul_ps(
					_mm_add_ps(
						_mm_set_ps(TRANS_INVERSE_TRANS_CAMERA_MATRIX.m13, TRANS_INVERSE_TRANS_CAMERA_MATRIX.m12, TRANS_INVERSE_TRANS_CAMERA_MATRIX.m13, TRANS_INVERSE_TRANS_CAMERA_MATRIX.m12),
						_mm_set_ps(p->position.y, p->position.x, p->position.y, p->position.x)
					),
					m128Tmp
				),
				_mm_set_ps(1.0f, 1.0f, TRANS_INVERSE_TRANS_CAMERA_MATRIX.m14, p->position.z) // second arg is NA
			);

		m128Tmp = _mm_set1_ps(p->scaleVal * p->scaleVal);
		
		tmp.v0_m128 = _mm_mul_ps(tmp.v0_m128, m128Tmp);
		tmp.v1_m128 = _mm_mul_ps(tmp.v1_m128, m128Tmp);
		tmp.v2.z = m128Tmp.m128_f32[0];
		tmp.v3_m128 = _mm_mul_ps(tmp.v3_m128, _mm_set1_ps(p->scaleVal));
		///////////////////////////////////////////////////////////////////////////////

		// use currrent matrix
		OpenGLDevice::SetTransformMatrixFloat((const float *)&tmp);

		// interate pointer
		p++;

		assert(p != nullptr);
	}while (p != tailParticle);
}

const Matrix ParticleEmitter::TRANS_CAMERA_MATRIX(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 5.0f, 40.0f, 1.0f
);

const Matrix ParticleEmitter::INVERSE_TRANS_CAMERA_MATRIX = Matrix::Inverse(TRANS_CAMERA_MATRIX);

const Matrix ParticleEmitter::TRANS_INVERSE_TRANS_CAMERA_MATRIX = Matrix::getTransMatrix(INVERSE_TRANS_CAMERA_MATRIX.v3_m128);

// --- End of File ---
