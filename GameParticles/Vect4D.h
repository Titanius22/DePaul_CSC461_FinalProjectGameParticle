//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef Vect4D_H
#define Vect4D_H

// includes
#include "Enum.h"

// Foward Declarations
class Matrix;

// class
class Vect4D
{
public:
	friend class Matrix;
	friend class ParticleEmitter;
	friend class Particle;

	Vect4D() = default;
	~Vect4D();
	Vect4D(const Vect4D&) = default;
	Vect4D& operator=(const Vect4D&) = default;
	// TODO: trial using _m for operator=

	Vect4D(const float tx, const float ty, const float tz, const float tw = 1.0f);
	Vect4D(const __m128& t);

	void Cross(const Vect4D &vin, Vect4D &vout) const;
	float &operator[](Vect e);
	void norm(Vect4D &out) const;

	Vect4D operator * (const float scale) const;
	Vect4D& operator *= (float scale);
	Vect4D operator - (const Vect4D& t) const;
	Vect4D operator + (const Vect4D& t) const;
	Vect4D& operator += (const Vect4D& t);
	union
	{
		struct
		{
			__m128 _m;
		};

		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
	};
};

#endif

// --- End of File ---
