//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#include "Vect4D.h"

Vect4D::Vect4D(const float tx, const float ty, const float tz, const float tw)
	: x(tx), y(ty), z(tz), w(tw)
{}

Vect4D::Vect4D(const __m128& t)
	: _m(t)
{}

Vect4D::~Vect4D()
{
	// nothing to delete
}

void Vect4D::norm(Vect4D &v) const
{
	float mag = sqrtf((this->x * this->x) + (this->y * this->y) + (this->z * this->z));

	if(0.0f < mag)
	{
		v.x = this->x / mag;
		v.y = this->y / mag;
		v.z = this->z / mag;
	}
}

Vect4D Vect4D::operator + (const Vect4D& t) const
{
	return Vect4D(this->x + t.x, this->y + t.y, this->z + t.z);
}

Vect4D& Vect4D::operator += (const Vect4D& t)
{
	this->x += t.x;
	this->y += t.y;
	this->z += t.z;
	return *this;
}

float &Vect4D::operator[](Vect e)
{
	switch(e)
	{
	case Vect::X:
		return x;
		break;
	case Vect::Y:
		return y;
		break;
	case Vect::Z:
		return z;
		break;
	case Vect::W:
		return w;
		break;
	default:
		assert(0);
		return x;
	}
}

Vect4D Vect4D::operator *(const float scale) const
{
	return Vect4D(this->x * scale, this->y * scale, this->z * scale);
}

Vect4D& Vect4D::operator *= (const float scale)
{
	this->x *= scale;
	this->y *= scale;
	this->z *= scale;
	return *this;
}

Vect4D Vect4D::operator - (const Vect4D& t) const
{
	return Vect4D(this->x - t.x, this->y - t.y, this->z - t.z);
}

void Vect4D::Cross(const Vect4D &vin, Vect4D &vout) const
{
	vout.x = (y * vin.z) - (z * vin.y);
	vout.y = (z * vin.x) - (x * vin.z);
	vout.z = (x * vin.y) - (y * vin.x);
}


// --- End of File ---
