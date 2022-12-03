//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#include <Math.h>
#include "Vect4d.h"
#include "Matrix.h"

Matrix::Matrix(){
	// can't set this to default.
}

Matrix::Matrix(
	const float _m0,  const float _m1,  const float _m2,  const float _m3,
	const float _m4,  const float _m5,  const float _m6,  const float _m7,
	const float _m8,  const float _m9,  const float _m10, const float _m11,
	const float _m12, const float _m13, const float _m14, const float _m15
)
	: m0(_m0),    m1(_m1),  m2(_m2),    m3(_m3),
	  m4(_m4),    m5(_m5),  m6(_m6),    m7(_m7),
	  m8(_m8),    m9(_m9),  m10(_m10), m11(_m11),
	  m12(_m12), m13(_m13), m14(_m14), m15(_m15)
{}

Matrix::Matrix(const __m128 _v0, const __m128 _v1, const __m128 _v2, const __m128 _v3)
	: v0_m128(_v0), v1_m128(_v1), v2_m128(_v2), v3_m128(_v3)
{}

Matrix::Matrix(const Vect4D _v0, const Vect4D _v1, const Vect4D _v2, const Vect4D _v3)
	: v0(_v0), v1(_v1), v2(_v2), v3(_v3)
{}


Matrix::~Matrix()
{
	// do nothing
}

void Matrix::setTransMatrix(const Vect4D *t)
{ 
	//	{	1		0		0		0	}
	//	{	0		1		0		0	}
	//	{	0		0		1		0	}
	//	{	x		y		z		1	}
	
	this->m0 = 1.0f;
	this->m1 = 0.0f;
	this->m2 = 0.0f;
	this->m3 = 0.0f;

	this->m4 = 0.0f;
	this->m5 = 1.0f;
	this->m6 = 0.0f;
	this->m7 = 0.0f;

	this->m8 = 0.0f;
	this->m9 = 0.0f;
	this->m10 = 1.0f;
	this->m11 = 0.0f;
	
	this->m12 = t->x;
	this->m13 = t->y;
	this->m14 = t->z;
	this->m15 = 1.0f;
}

// only runs once, don't care to optimize
Matrix Matrix::getTransMatrix(const Vect4D t)
{
	//	{	1		0		0		0	}
	//	{	0		1		0		0	}
	//	{	0		0		1		0	}
	//	{	x		y		z		1	}

	Matrix tmp;

	tmp.m0 = 1.0f;
	tmp.m1 = 0.0f;
	tmp.m2 = 0.0f;
	tmp.m3 = 0.0f;

	tmp.m4 = 0.0f;
	tmp.m5 = 1.0f;
	tmp.m6 = 0.0f;
	tmp.m7 = 0.0f;

	tmp.m8 = 0.0f;
	tmp.m9 = 0.0f;
	tmp.m10 = 1.0f;
	tmp.m11 = 0.0f;

	tmp.m12 = t.x;
	tmp.m13 = t.y;
	tmp.m14 = t.z;
	tmp.m15 = 1.0f;

	return tmp;
}

void Matrix::set(const MatrixRow row, Vect4D *t )
{
	switch( row )
	{
	case MatrixRow::MATRIX_ROW_0:
		this->m0 = t->x;
		this->m1 = t->y;
		this->m2 = t->z;
		this->m3 = t->w;
		break;

	case MatrixRow::MATRIX_ROW_1:
		this->m4 = t->x;
		this->m5 = t->y;
		this->m6 = t->z;
		this->m7 = t->w;
		break;

	case MatrixRow::MATRIX_ROW_2:
		this->m8 = t->x;
		this->m9 = t->y;
		this->m10 = t->z;
		this->m11 = t->w;
		break;

	case MatrixRow::MATRIX_ROW_3:
		this->m12 = t->x;
		this->m13 = t->y;
		this->m14 = t->z;
		this->m15 = t->w;
		break;

	default:
		// should never get here, if we are here something bad has happened
		assert(0);
	}
}

float &Matrix::operator[](Index e)
{
	// get the individual elements
	switch(e)
	{
	case Index::m0:
		return m0;
		break;
	case Index::m1:
		return m1;
		break;
	case Index::m2:
		return m2;
		break;
	case Index::m3:
		return m3;
		break;
	case Index::m4:
		return m4;
		break;
	case Index::m5:
		return m5;
		break;
	case Index::m6:
		return m6;
		break;
	case Index::m7:
		return m7;
		break;
	case Index::m8:
		return m8;
		break;
	case Index::m9:
		return m9;
		break;	
	case Index::m10:
		return m10;
		break;
	case Index::m11:
		return m11;
		break;	
	case Index::m12:
		return m12;
		break;	
	case Index::m13:
		return m13;
		break;	
	case Index::m14:
		return m14;
		break;
	case Index::m15:
		return m15;
		break;
	default:
		assert(0);
		return m0;
		break;
	}
}

void Matrix::get(const MatrixRow row, Vect4D *t ) const
{ // get a row of the matrix
	switch( row )
	{
	case MatrixRow::MATRIX_ROW_0:
		t->x = this->m0;
		t->y = this->m1;
		t->z = this->m2;
		t->w = this->m3;
		break;

	case MatrixRow::MATRIX_ROW_1:
		t->x = this->m4;
		t->y = this->m5;
		t->z = this->m6;
		t->w = this->m7;
		break;

	case MatrixRow::MATRIX_ROW_2:
		t->x = this->m8;
		t->y = this->m9;
		t->z = this->m10;
		t->w = this->m11;
		break;

	case MatrixRow::MATRIX_ROW_3:
		t->x = this->m12;
		t->y = this->m13;
		t->z = this->m14;
		t->w = this->m15;
		break;

	default:
		assert(0);
	}
}

Matrix Matrix::operator*(const Matrix& rhs) const
{ 
	Matrix tmp;
	
	tmp.m0 = (m0*rhs.m0) + (m1*rhs.m4) + (m2*rhs.m8) + (m3*rhs.m12);
	tmp.m1 = (m0*rhs.m1) + (m1*rhs.m5) + (m2*rhs.m9) + (m3*rhs.m13);
	tmp.m2 = (m0*rhs.m2) + (m1*rhs.m6) + (m2*rhs.m10) + (m3*rhs.m14);
	tmp.m3 = (m0*rhs.m3) + (m1*rhs.m7) + (m2*rhs.m11) + (m3*rhs.m15);
	
	tmp.m4 = (m4*rhs.m0) + (m5*rhs.m4) + (m6*rhs.m8) + (m7*rhs.m12);
	tmp.m5 = (m4*rhs.m1) + (m5*rhs.m5) + (m6*rhs.m9) + (m7*rhs.m13);
	tmp.m6 = (m4*rhs.m2) + (m5*rhs.m6) + (m6*rhs.m10) + (m7*rhs.m14);
	tmp.m7 = (m4*rhs.m3) + (m5*rhs.m7) + (m6*rhs.m11) + (m7*rhs.m15);
	
	tmp.m8 = (m8*rhs.m0) + (m9*rhs.m4) + (m10*rhs.m8) + (m11*rhs.m12);
	tmp.m9 = (m8*rhs.m1) + (m9*rhs.m5) + (m10*rhs.m9) + (m11*rhs.m13);
	tmp.m10 = (m8*rhs.m2) + (m9*rhs.m6) + (m10*rhs.m10) + (m11*rhs.m14);
	tmp.m11 = (m8*rhs.m3) + (m9*rhs.m7) + (m10*rhs.m11) + (m11*rhs.m15);
	
	tmp.m12 = (m12*rhs.m0) + (m13*rhs.m4) + (m14*rhs.m8) + (m15*rhs.m12);
	tmp.m13 = (m12*rhs.m1) + (m13*rhs.m5) + (m14*rhs.m9) + (m15*rhs.m13);
	tmp.m14 = (m12*rhs.m2) + (m13*rhs.m6) + (m14*rhs.m10) + (m15*rhs.m14);
	tmp.m15 = (m12*rhs.m3) + (m13*rhs.m7) + (m14 *rhs.m11) + (m15*rhs.m15);

	return tmp;
}

Matrix Matrix::operator-(const Matrix& rhs) const
{
	return Matrix(
		 m0 - rhs.m0,   m1 - rhs.m1,   m2 - rhs.m2,   m3 - rhs.m3,
		 m4 - rhs.m4,   m5 - rhs.m5,   m6 - rhs.m6,   m7 - rhs.m7,
		 m8 - rhs.m8,   m9 - rhs.m9,  m10 - rhs.m10, m11 - rhs.m11,
		m12 - rhs.m12, m13 - rhs.m13, m14 - rhs.m14, m15 - rhs.m15
	);
}

Matrix& Matrix::operator/=(const float rhs)
{ 
	// divide each element by a value
	// using inverse multiply trick, faster that individual divides
	float inv_rhs = 1.0f/rhs;

	m0 *= inv_rhs;
	m1 *= inv_rhs;
	m2 *= inv_rhs;
	m3 *= inv_rhs;
	m4 *= inv_rhs;
	m5 *= inv_rhs;
	m6 *= inv_rhs;
	m7 *= inv_rhs;
	m8 *= inv_rhs;
	m9 *= inv_rhs;
	m10 *= inv_rhs;
	m11 *= inv_rhs;
	m12 *= inv_rhs;
	m13 *= inv_rhs;
	m14 *= inv_rhs;
	m15 *= inv_rhs;

	return *this;
}

// only runs twice, don't care to optimize
float Matrix::Determinant() const
{
	float ta = (m10 * m15) - (m11 * m14);
	// tb = (kq - mo)
	float tb = (m9 * m15) - (m11 * m13);
	// tc = (kp - lo)
	float tc = (m9 * m14) - (m10 * m13);
	// td = (jq - mn)
	float td = (m8 * m15) - (m11 * m12);
	// te = (jo - kn)
	float te = (m8 * m13) - (m9 *  m12);
	// tf = (jp - ln)
	float tf = (m8 * m14) - (m10 * m12);
	
	return ((m0 * ((m5 * ta) - (m6 * tb) + (m7 * tc)))
			- (m1 * ((m4 * ta) - (m6 * td) + (m7 * tf)))
			+ (m2 * ((m4 * tb) - (m5 * td) + (m7 * te)))
			- (m3 * ((m4 * tc) - (m5 * tf) + (m6 * te))));
	
}

// only runs twice, don't care to optimize
Matrix Matrix::GetAdjugate() const
{
	Matrix tmp;
	
	// load		ABC		(3)		ABC--
	float t1 = (m10*m15) - (m11*m14);
	float t2 = (m9*m15) - (m11*m13);
	float t3 = (m9*m14) - (m10*m13);
	
	// a = f(ta) - g(tb) + h(tc)
	tmp.m0 = (m5*t1) - (m6*t2) + (m7*t3);
	// b = -( b(ta) - c(tb) + d(tc))
	tmp.m1 = -((m1*t1) - (m2*t2) + (m3*t3));
	
	// load		JK		(5)		ABCJK
	float t4 = (m8*m15) - (m11*m12);
	float t5 = (m8*m14) - (m10*m12);
	// e = - ( e(ta) - g(tj) + h(tk))
	tmp.m4 = -( (m4*t1) - (m6*t4) + (m7*t5));
	// f = a(ta) - c(tj) + d(tk)
	tmp.m5 = (m0*t1) - (m2*t4) + (m3*t5);
	
	// unload	AJ		(3)		-BC-K
	// load		P		(4)		PBC-K
	t1 = (m8*m13) - (m9*m12);
	// n = -( e(tc) - f(tk) + g(tp))
	tmp.m12 = -((m4*t3) - (m5*t5) + (m6*t1));
	// o = a(tc) - b(tk) + c(tp)
	tmp.m13 = (m0*t3) - (m1*t5) + (m2*t1);
	
	// unload	KC		(2)		PB---
	// load		J		(3)		PBJ--
	t3 = (m8*m15) - (m11*m12);
	
	// j = e(tb) - f(tj) + h(tp)
	tmp.m8 = (m4*t2) - (m5*t3) + (m7*t1);
	// k = - ( a(tb) - b(tj) + d(tp))
	tmp.m9 = -((m0*t2) - (m1*t3) + (m3*t1));
	
	// unload	BPJ		(0)		-----
	// load		DLM		(3)		DLM--
	t1 = (m6*m15) - (m7*m14);
	t2 = (m4*m15) - (m7*m12);
	t3 = (m4*m14) - (m6*m12);
	
	// g = - ( a(td) - c(tl) + d(tm))
	tmp.m6 = -((m0*t1) - (m2*t2) + (m3*t3));
	
	// load		FR		(5)		DLMFR
	t4 = (m5*m14) - (m6*m13);
	t5 = (m4*m13) - (m5*m12);
	
	// p = - ( a(tf) - b(tm) + c(tr))
	tmp.m14 = -( (m0*t4) - (m1*t3) + (m3*t5));
	
	// unload	M		(4)		DL-FR
	// load		E		(5)		DLEFR
	t3 = (m5*m15) - (m7*m13);
	
	// l = a(te) - b(tl) + d(tr)
	tmp.m10 = (m0*t3) - (m1*t2) + (m3*t5);
	
	// unload	LR		(3)		D-EF-
	// c = b(td) - c(te) + d(tf)
	tmp.m2 = (m1*t1) - (m2*t3) + (m3*t4);
	
	// unload	DEF		(0)		-----
	// load		GHI		(3)		GHI--
	t1 = (m6*m11) - (m7*m10);
	t2 = (m5*m11) - (m7*m9);
	t3 = (m5*m10) - (m6*m9);
	
	// d = -( b(tg) - c(th) + d(ti) )
	tmp.m3 = -( (m1*t1) - (m2*t2) + (m3*t3));
	
	// load		NO		(5)		GHINO
	t4 = (m4*m11) - (m7*m8);
	t5 = (m4*m10) - (m6*m8);
	
	// h = a(tg) - c(tn) + d(to)
	tmp.m7 = (m0*t1) - (m2*t4) + (m3*t5);
	
	// unload	G		(4)		-HINO
	// load		Q		(5)		QHINO
	t1 = (m4*m9) - (m5*m8);
	
	// m = -( a(th) - b(tn) + d(tq))
	tmp.m11 = -((m0*t2) - (m1*t4) + (m3*t1));
	
	// unload	HN		(3)		Q-I-O
	// q = a(ti) - b(to) + c(tq)
	tmp.m15 = (m0*t3) - (m1*t5) + (m2*t1);
	
	return tmp;
}

// only runs twice, don't care to optimize
Matrix Matrix::Inverse(const Matrix in)
{
	Matrix tmp;
	float det = in.Determinant();
	if(fabs(det) >= 0.0001)
	{
		tmp = in.GetAdjugate();
		tmp /= det;
	}
	else
	{
		// do nothing, Matrix is not invertable
		std::cout << "what is happening";
	}

	return tmp;
}

void Matrix::setRotZMatrix(const float az)
{
	//	{	cos		-sin	0		0	}
	//	{	sin		cos		0		0	}
	//	{	0		0		1		0	}
	//	{	0		0		0		1	}
	
	this->m0 = cos(az);
	this->m1 = -sin(az);
	this->m2 = 0;
	this->m3 = 0;
	
	this->m4 = sin(az);
	this->m5 = cos(az);
	this->m6 = 0;
	this->m7 = 0;
	
	this->m8 = 0;
	this->m9 = 0;
	this->m10 = 1;
	this->m11 = 0;
	
	this->m12 = 0;
	this->m13 = 0;
	this->m14 = 0;
	this->m15 = 1;
}

void Matrix::setScaleMatrix(const Vect4D *scale)
{
	//	{	sx		0		0		0	}
	//	{	0		sy		0		0	}
	//	{	0		0		sz		0	}
	//	{	0		0		0		1	}
	
	this->m0 = scale->x;
	this->m1 = 0;
	this->m2 = 0;
	this->m3 = 0;
	
	this->m4 = 0;
	this->m5 = scale->y;
	this->m6 = 0;
	this->m7 = 0;
	
	this->m8 = 0;
	this->m9 = 0;
	this->m10 = scale->z;
	this->m11 = 0;
	
	this->m12 = 0;
	this->m13 = 0;
	this->m14 = 0;
	this->m15 = 1.0;
}

// --- End of File ---
