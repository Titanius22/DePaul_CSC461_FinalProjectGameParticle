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
	headParticle(nullptr),
	vel_variance(-29.0f * 0.0001f, 0.70f * 0.0001f, -1.0f * 0.001f),
	pos_variance(-3.50f * 0.0001f, 3.50f * 0.0001f, 5.0f * 0.001f),
	scale_variance(3.0f)
{
	//assert(false);
	//std::cout << "Update this for sequencial memory";
	//headParticle = (Particle*)(new unsigned char [NUM_PARTICLES*sizeof(Particle)]);

}

ParticleEmitter::~ParticleEmitter()
{
	Particle* pTmp = this->headParticle;
	Particle* pDeleteMe;
	while (pTmp != nullptr)
	{
		pDeleteMe = pTmp;
		pTmp = pTmp->next;
		delete pDeleteMe;
	}
}

void ParticleEmitter::SpawnParticle()
{
	// create AND initialize new particle
	Particle *newParticle = new Particle(this);

	// increment count
	last_active_particle++;

	// add to list
	this->addParticleToList( newParticle );
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
	Particle* pParticleToDelete;

	while( p != nullptr )
	{
		// call every particle and update its position 
		p->Update(time_elapsed);

		// if life is greater that the max_life 
		// and there is some left on the list
		// remove node
		if((last_active_particle > 0) && (p->life > MAX_LIFE))
		{
			pParticleToDelete = p;

			/////////////////////////////////////////
			Particle* plot = this->headParticle;
			float oldest = plot->life;
			float youngest = plot->life;
			while (plot != nullptr)
			{
				if (plot->life > oldest)
				{
					oldest = plot->life;
				}
				if (plot->life < youngest)
				{
					youngest = plot->life;
				}

				plot = plot->next;
			}
			////////////////////////////////////////
			
			// increment to next point
			p = p->next;

			// remove prev node
			this->removeParticleFromList(pParticleToDelete);

			// update the number of particles
			last_active_particle--;
		}
		else
		{
			// increment to next point
			p = p->next;
		}

	}

	// make sure the counts track (asserts go away in release - relax Christos)
	last_loop = current_time;
}

void ParticleEmitter::draw() const
{
	Matrix tmp;

	// iterate throught the list of particles
	Particle* p = this->headParticle;
	while(p != nullptr)
	{
		assert(p != nullptr);

		// particle position
		Matrix transParticle;
		transParticle.setTransMatrix(&(p->position));

		// rotation matrix
		Matrix rotParticle;
		rotParticle.setRotZMatrix(p->rotation);

		// pivot Point
		Vect4D pivotVect(-20.0f, 0.0f, 200.0f);
		pivotVect *= p->life;
		Matrix pivotParticle;
		pivotParticle.setTransMatrix( &pivotVect );

		// scale Matrix
		Matrix scaleMatrix;
		scaleMatrix.setScaleMatrix( &(p->scale) );
		
		// total transformation of particle
		tmp = scaleMatrix * TRANS_INVERSE_TRANS_CAMERA_MATRIX * transParticle * rotParticle * scaleMatrix;

		// use currrent matrix
		OpenGLDevice::SetTransformMatrixFloat((const float *)&tmp);

		// interate pointer
		p = p->next;
	}

	assert(p == nullptr);
}

void ParticleEmitter::addParticleToList(Particle* p)
{
	assert(p);
	if (this->headParticle == nullptr)
	{ // first on list
		this->headParticle = p;
		p->next = nullptr;
		p->prev = nullptr;
	}
	else
	{ // add to front of list
		headParticle->prev = p;
		p->next = headParticle;
		p->prev = nullptr;
		headParticle = p;
	}
}

void ParticleEmitter::removeParticleFromList(Particle* p)
{
	// make sure we are not screwed with a null pointer
	assert(p);

	if (p->prev == nullptr && p->next == nullptr)
	{ // only one on the list
		this->headParticle = nullptr;
	}
	else if (p->prev == nullptr && p->next != nullptr)
	{ // first on the list
		p->next->prev = nullptr;
		this->headParticle = p->next;
	}
	else if (p->prev != nullptr && p->next == nullptr)
	{ // last on the list 
		p->prev->next = nullptr;
	}
	else//( p->next != nullptr  && p->prev !=nullptr )
	{ // middle of the list
		p->prev->next = p->next;
		p->next->prev = p->prev;
	}

	// bye bye
	delete p;
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
