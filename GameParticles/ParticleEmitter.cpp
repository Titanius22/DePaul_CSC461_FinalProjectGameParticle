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
	pParticleBlockStart((Particle*)(new unsigned char [NUM_PARTICLES*sizeof(Particle)])),
	pParticleBlockEnd(pParticleBlockStart + NUM_PARTICLES),
	headParticle(pParticleBlockStart),
	tailParticle(headParticle)
{}

ParticleEmitter::~ParticleEmitter()
{
	delete pParticleBlockStart;
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
	Matrix tmp;

	// particle position
	Matrix transParticle(Matrix::IDENTITY_MATRIX);

	// rotation matrix
	Matrix rotParticle(Matrix::IDENTITY_MATRIX);

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
		
		// particle position
		//transParticle.setTransMatrix(&(p->position));

		// rotation matrix
		//rotParticle.setRotZMatrix(p->rotation);
		
		// total transformation of particle
		//tmp = p->scaleMatrix * TRANS_INVERSE_TRANS_CAMERA_MATRIX * transParticle * rotParticle * p->scaleMatrix;
		tmp = superMegaFastMatrixCalculator(p->scaleVect, p->position, p->rotation);

		// use currrent matrix
		OpenGLDevice::SetTransformMatrixFloat((const float *)&tmp);

		// interate pointer
		p++;

		assert(p != nullptr);
	}while (p != tailParticle);
}

Matrix ParticleEmitter::superMegaFastMatrixCalculator(Vect4D _scaleVec, Vect4D _posVec, float _rotation) const
{
	float cosVar = cos(_rotation);
	float sinVar = sin(_rotation);

	Matrix ma1ma2ma3ma4;
	ma1ma2ma3ma4.v0_m128 = _mm_set_ps(0.0f, 0.0f, sinVar, cosVar);
	ma1ma2ma3ma4.v1_m128 = _mm_set_ps(0.0f, 0.0f, cosVar, -sinVar);
	ma1ma2ma3ma4.v2_m128 = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
	ma1ma2ma3ma4.v3_m128 =
		_mm_hadd_ps(
			_mm_mul_ps(
				//_mm_set_ps(ma1ma2.m13, ma1ma2.m12, ma1ma2.m13, ma1ma2.m12),
				_mm_add_ps(
					_mm_set_ps(TRANS_INVERSE_TRANS_CAMERA_MATRIX.m13, TRANS_INVERSE_TRANS_CAMERA_MATRIX.m12, TRANS_INVERSE_TRANS_CAMERA_MATRIX.m13, TRANS_INVERSE_TRANS_CAMERA_MATRIX.m12),
					_mm_set_ps(_posVec.y, _posVec.x, _posVec.y, _posVec.x)
				),
				_mm_set_ps(cosVar, -sinVar, sinVar, cosVar)
			),
			_mm_set_ps(1.0f, 1.0f, TRANS_INVERSE_TRANS_CAMERA_MATRIX.m14, _posVec.z) // second arg is NA
		);

	ma1ma2ma3ma4.v0_m128 = _mm_mul_ps(ma1ma2ma3ma4.v0_m128, _scaleVec._m);
	ma1ma2ma3ma4.v1_m128 = _mm_mul_ps(ma1ma2ma3ma4.v1_m128, _scaleVec._m);
	//ma1ma2ma3ma4.v2.z = _scaleVec.z; this operation was combined below
	ma1ma2ma3ma4.v3_m128 = _mm_mul_ps(ma1ma2ma3ma4.v3_m128, _scaleVec._m);

	ma1ma2ma3ma4.v0_m128 = _mm_mul_ps(ma1ma2ma3ma4.v0_m128, _mm_set1_ps(_scaleVec.x));
	ma1ma2ma3ma4.v1_m128 = _mm_mul_ps(ma1ma2ma3ma4.v1_m128, _mm_set1_ps(_scaleVec.y));
	ma1ma2ma3ma4.v2.z = _scaleVec.z * _scaleVec.z; // combined with above
	// no need. this would be xxx*1 

	return ma1ma2ma3ma4;
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
