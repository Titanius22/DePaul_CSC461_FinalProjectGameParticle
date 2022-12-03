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
	/*Particle* pTmp = this->headParticle;
	Particle* pDeleteMe;
	while (pTmp != nullptr)
	{
		pDeleteMe = pTmp;
		pTmp = pTmp->next;
		delete pDeleteMe;
	}*/
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

	// add to list
	//this->addParticleToList( newParticle );
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

	// temp Particle
	//Particle* pParticleToDelete;

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
		if((last_active_particle > 0) && (p->life > MAX_LIFE))
		{
			assert(p == headParticle);

			headParticle++;
			if (headParticle == pParticleBlockEnd)
			{
				headParticle = pParticleBlockStart;
			}
			
			// remove prev node
			//this->removeParticleFromList(pParticleToDelete);

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
	Matrix transParticle;

	// rotation matrix
	Matrix rotParticle;

	// pivot Point
	Vect4D pivotVect(-20.0f, 0.0f, 200.0f);
	Matrix pivotParticle;

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
		transParticle.setTransMatrix(&(p->position));

		// rotation matrix
		rotParticle.setRotZMatrix(p->rotation);

		// pivot Point
		pivotVect *= p->life;
		pivotParticle.setTransMatrix( &pivotVect );
		
		// total transformation of particle
		tmp = p->scaleMatrix * TRANS_INVERSE_TRANS_CAMERA_MATRIX * transParticle * rotParticle * p->scaleMatrix;

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
