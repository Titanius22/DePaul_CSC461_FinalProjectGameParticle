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
{

}

Particle::~Particle()
{
	// nothing to do
}

//void Particle::CopyDataOnly( Particle *p )
//{
//	// copy the data only
//	// this way of copying data is more efficient that element by element
//	this->position = p->position;
//	this->velocity = p->velocity;
//	this->scale    = p->scale;
//	this->rotation = p->rotation;
//	this->rotation_velocity = p->rotation_velocity;
//	this->life     = p->life;
//}

void Particle::Update(const float& time_elapsed)
{
	// Rotate the matrices
	prev_matrix = curr_matrix;
	//prev_Row0 = curr_Row0;
	//prev_Row1 = curr_Row1;
	//prev_Row2 = curr_Row2;
	//prev_Row3 = curr_Row3;

	//Matrix tmp(
	//	diff_Row0._m, diff_Row1._m, diff_Row2._m, diff_Row3._m
	//);

	//Matrix tmp(diff_matrix);

	//tmp.set(Matrix::MatrixRow::MATRIX_ROW_0, &this->diff_Row0);
	//tmp.set(Matrix::MatrixRow::MATRIX_ROW_1, &this->diff_Row1);
	//tmp.set(Matrix::MatrixRow::MATRIX_ROW_2, &this->diff_Row2);
	//tmp.set(Matrix::MatrixRow::MATRIX_ROW_3, &this->diff_Row3);

	float MatrixScale = -1.3f * (diff_matrix.Determinant());

	// serious math below - magic secret sauce
	life += time_elapsed;
	position = position + (velocity * time_elapsed);
	Vect4D v;
	position.Cross(Z_AXIS, v);
	v.norm(v);
	position = position + (v * 0.05f * life);

	if( MatrixScale > 1.0 )
	{
		MatrixScale = 1.0f/MatrixScale;
	};

	rotation = rotation + MatrixScale + (rotation_velocity * time_elapsed) ;
}

// --- End of File ---
