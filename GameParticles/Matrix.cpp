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

const Matrix Matrix::IDENTITY_MATRIX(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
);

void Matrix::setIdentityMatrix()
{
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

	this->m12 = 0.0f;
	this->m13 = 0.0f;
	this->m14 = 0.0f;
	this->m15 = 1.0f;
}

void Matrix::setTransMatrix(const Vect4D *t)
{ 
	//	{	1		0		0		0	}
	//	{	0		1		0		0	}
	//	{	0		0		1		0	}
	//	{	x		y		z		1	}
	
	// doing asserts because all elements except 
	// m12, m13, m14 should be set to identity_matrix values

	assert(this->m0 == 1.0f);
	assert(this->m1 == 0.0f);
	assert(this->m2 == 0.0f);
	assert(this->m3 == 0.0f);

	assert(this->m4 == 0.0f);
	assert(this->m5 == 1.0f);
	assert(this->m6 == 0.0f);
	assert(this->m7 == 0.0f);

	assert(this->m8 == 0.0f);
	assert(this->m9 == 0.0f);
	assert(this->m10 == 1.0f);
	assert(this->m11 == 0.0f);
	
	this->m12 = t->x;
	this->m13 = t->y;
	this->m14 = t->z;
	assert(this->m15 == 1.0f);
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
	
	// doing asserts because all elements except 
	// m0, m1, m4, m5 should be set to identity_matrix values
	
	this->m0 = cos(az);
	this->m1 = -sin(az);
	assert(this->m2 == 0);
	assert(this->m3 == 0);

	this->m4 = sin(az);
	this->m5 = cos(az);
	assert(this->m6 == 0);
	assert(this->m7 == 0);

	assert(this->m8 == 0);
	assert(this->m9 == 0);
	assert(this->m10 == 1);
	assert(this->m11 == 0);

	assert(this->m12 == 0);
	assert(this->m13 == 0);
	assert(this->m14 == 0);
	assert(this->m15 == 1);
}

// only run when particle is created
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

// Proxy stuff
MmulM::operator Matrix()
{
	Matrix returnVal;
	for (int i = 0; i < 4; i++)
	{
		((Vect4D*)&returnVal + i)->_m = _mm_add_ps(
			_mm_add_ps(
				_mm_mul_ps(ma2.v0_m128, _mm_set_ps1(((Vect4D*)&ma1 + i)->x)),
				_mm_mul_ps(ma2.v1_m128, _mm_set_ps1(((Vect4D*)&ma1 + i)->y))
			),
			_mm_add_ps(
				_mm_mul_ps(ma2.v2_m128, _mm_set_ps1(((Vect4D*)&ma1 + i)->z)),
				_mm_mul_ps(ma2.v3_m128, _mm_set_ps1(((Vect4D*)&ma1 + i)->w))
			)
		);
	}
	return returnVal;
}

//MmulMmulMmulMmulM::operator Matrix()
//{
//	// returnVal1 = m1*m2
//	Matrix returnVal1;
//	for (int i = 0; i < 4; i++)
//	{
//		((Vect4D*)&returnVal1 + i)->_m = _mm_add_ps(
//			_mm_add_ps(
//				_mm_mul_ps(ma2.v0_m128, _mm_set_ps1(((Vect4D*)&ma1 + i)->x)),
//				_mm_mul_ps(ma2.v1_m128, _mm_set_ps1(((Vect4D*)&ma1 + i)->y))
//			),
//			_mm_add_ps(
//				_mm_mul_ps(ma2.v2_m128, _mm_set_ps1(((Vect4D*)&ma1 + i)->z)),
//				_mm_mul_ps(ma2.v3_m128, _mm_set_ps1(((Vect4D*)&ma1 + i)->w))
//			)
//		);
//	}
//
//
//	// returnVal2 = returnVal1*m3
//	Matrix returnVal2;
//	for (int i = 0; i < 4; i++)
//	{
//		((Vect4D*)&returnVal2 + i)->_m = _mm_add_ps(
//			_mm_add_ps(
//				_mm_mul_ps(ma3.v0_m128, _mm_set_ps1(((Vect4D*)&returnVal1 + i)->x)),
//				_mm_mul_ps(ma3.v1_m128, _mm_set_ps1(((Vect4D*)&returnVal1 + i)->y))
//			),
//			_mm_add_ps(
//				_mm_mul_ps(ma3.v2_m128, _mm_set_ps1(((Vect4D*)&returnVal1 + i)->z)),
//				_mm_mul_ps(ma3.v3_m128, _mm_set_ps1(((Vect4D*)&returnVal1 + i)->w))
//			)
//		);
//	}
//
//
//	// returnVal1 = returnVal2*m4
//	for (int i = 0; i < 4; i++)
//	{
//		((Vect4D*)&returnVal1 + i)->_m = _mm_add_ps(
//			_mm_add_ps(
//				_mm_mul_ps(ma4.v0_m128, _mm_set_ps1(((Vect4D*)&returnVal2 + i)->x)),
//				_mm_mul_ps(ma4.v1_m128, _mm_set_ps1(((Vect4D*)&returnVal2 + i)->y))
//			),
//			_mm_add_ps(
//				_mm_mul_ps(ma4.v2_m128, _mm_set_ps1(((Vect4D*)&returnVal2 + i)->z)),
//				_mm_mul_ps(ma4.v3_m128, _mm_set_ps1(((Vect4D*)&returnVal2 + i)->w))
//			)
//		);
//	}
//
//
//	// returnVal2 = returnVal1*m4
//	for (int i = 0; i < 4; i++)
//	{
//		((Vect4D*)&returnVal2 + i)->_m = _mm_add_ps(
//			_mm_add_ps(
//				_mm_mul_ps(ma5.v0_m128, _mm_set_ps1(((Vect4D*)&returnVal1 + i)->x)),
//				_mm_mul_ps(ma5.v1_m128, _mm_set_ps1(((Vect4D*)&returnVal1 + i)->y))
//			),
//			_mm_add_ps(
//				_mm_mul_ps(ma5.v2_m128, _mm_set_ps1(((Vect4D*)&returnVal1 + i)->z)),
//				_mm_mul_ps(ma5.v3_m128, _mm_set_ps1(((Vect4D*)&returnVal1 + i)->w))
//			)
//		);
//	}
//
//
//	// returnVal2 = ma1*ma2*ma3*ma4*ma5
//	return returnVal2;
//}

//MmulMmulMmulMmulM::operator Matrix()
//{
//	// returnVal2 = m3*m4
//	Matrix returnVal2;
//	for (int i = 0; i < 4; i++)
//	{
//		((Vect4D*)&returnVal2 + i)->_m = _mm_add_ps(
//			_mm_add_ps(
//				_mm_mul_ps(ma4.v0_m128, _mm_set_ps1(((Vect4D*)&ma3 + i)->x)),
//				_mm_mul_ps(ma4.v1_m128, _mm_set_ps1(((Vect4D*)&ma3 + i)->y))
//			),
//			_mm_add_ps(
//				_mm_mul_ps(ma4.v2_m128, _mm_set_ps1(((Vect4D*)&ma3 + i)->z)),
//				_mm_mul_ps(ma4.v3_m128, _mm_set_ps1(((Vect4D*)&ma3 + i)->w))
//			)
//		);
//	}
//	
//	
//	// returnVal1 = m1*m2
//	Matrix returnVal1;
//	for (int i = 0; i < 4; i++)
//	{
//		((Vect4D*)&returnVal1 + i)->_m = _mm_add_ps(
//			_mm_add_ps(
//				_mm_mul_ps(ma2.v0_m128, _mm_set_ps1(((Vect4D*)&ma1 + i)->x)),
//				_mm_mul_ps(ma2.v1_m128, _mm_set_ps1(((Vect4D*)&ma1 + i)->y))
//			),
//			_mm_add_ps(
//				_mm_mul_ps(ma2.v2_m128, _mm_set_ps1(((Vect4D*)&ma1 + i)->z)),
//				_mm_mul_ps(ma2.v3_m128, _mm_set_ps1(((Vect4D*)&ma1 + i)->w))
//			)
//		);
//	}
//
//
//	// returnVal2 = returnVal2*m5
//	for (int i = 0; i < 4; i++)
//	{
//		((Vect4D*)&returnVal2 + i)->_m = _mm_add_ps(
//			_mm_add_ps(
//				_mm_mul_ps(ma5.v0_m128, _mm_set_ps1(((Vect4D*)&returnVal2 + i)->x)),
//				_mm_mul_ps(ma5.v1_m128, _mm_set_ps1(((Vect4D*)&returnVal2 + i)->y))
//			),
//			_mm_add_ps(
//				_mm_mul_ps(ma5.v2_m128, _mm_set_ps1(((Vect4D*)&returnVal2 + i)->z)),
//				_mm_mul_ps(ma5.v3_m128, _mm_set_ps1(((Vect4D*)&returnVal2 + i)->w))
//			)
//		);
//	}
//
//
//	// returnVal1 = returnVal1*returnVal2
//	for (int i = 0; i < 4; i++)
//	{
//		((Vect4D*)&returnVal1 + i)->_m = _mm_add_ps(
//			_mm_add_ps(
//				_mm_mul_ps(returnVal2.v0_m128, _mm_set_ps1(((Vect4D*)&returnVal1 + i)->x)),
//				_mm_mul_ps(returnVal2.v1_m128, _mm_set_ps1(((Vect4D*)&returnVal1 + i)->y))
//			),
//			_mm_add_ps(
//				_mm_mul_ps(returnVal2.v2_m128, _mm_set_ps1(((Vect4D*)&returnVal1 + i)->z)),
//				_mm_mul_ps(returnVal2.v3_m128, _mm_set_ps1(((Vect4D*)&returnVal1 + i)->w))
//			)
//		);
//	}
//
//
//	// returnVal1 = ma1*ma2*ma3*ma4*ma5
//	return returnVal1;
//}

//MmulMmulMmulMmulM::operator Matrix()
//{
//	// returnVal2 = m3*m4
//	Matrix returnVal2;
//	returnVal2.v0_m128 = _mm_add_ps(
//		_mm_add_ps(
//			_mm_mul_ps(ma4.v0_m128, _mm_set_ps1(ma3.v0.x)),
//			_mm_mul_ps(ma4.v1_m128, _mm_set_ps1(ma3.v0.y))
//		),
//		_mm_add_ps(
//			_mm_mul_ps(ma4.v2_m128, _mm_set_ps1(ma3.v0.z)),
//			_mm_mul_ps(ma4.v3_m128, _mm_set_ps1(ma3.v0.w))
//		)
//	);
//	returnVal2.v1_m128 = _mm_add_ps(
//		_mm_add_ps(
//			_mm_mul_ps(ma4.v0_m128, _mm_set_ps1(ma3.v1.x)),
//			_mm_mul_ps(ma4.v1_m128, _mm_set_ps1(ma3.v1.y))
//		),											 
//		_mm_add_ps(									 
//			_mm_mul_ps(ma4.v2_m128, _mm_set_ps1(ma3.v1.z)),
//			_mm_mul_ps(ma4.v3_m128, _mm_set_ps1(ma3.v1.w))
//		)
//	);
//	returnVal2.v2_m128 = _mm_add_ps(
//		_mm_add_ps(
//			_mm_mul_ps(ma4.v0_m128, _mm_set_ps1(ma3.v2.x)),
//			_mm_mul_ps(ma4.v1_m128, _mm_set_ps1(ma3.v2.y))
//		),										
//		_mm_add_ps(								
//			_mm_mul_ps(ma4.v2_m128, _mm_set_ps1(ma3.v2.z)),
//			_mm_mul_ps(ma4.v3_m128, _mm_set_ps1(ma3.v2.w))
//		)
//	);
//	returnVal2.v3_m128 = _mm_add_ps(
//		_mm_add_ps(
//			_mm_mul_ps(ma4.v0_m128, _mm_set_ps1(ma3.v3.x)),
//			_mm_mul_ps(ma4.v1_m128, _mm_set_ps1(ma3.v3.y))
//		),										
//		_mm_add_ps(								
//			_mm_mul_ps(ma4.v2_m128, _mm_set_ps1(ma3.v3.z)),
//			_mm_mul_ps(ma4.v3_m128, _mm_set_ps1(ma3.v3.w))
//		)
//	);
//
//
//	// returnVal1 = m1*m2
//	Matrix returnVal1;
//	returnVal1.v0_m128 = _mm_add_ps(
//		_mm_add_ps(
//			_mm_mul_ps(ma2.v0_m128, _mm_set_ps1(ma1.v0.x)),
//			_mm_mul_ps(ma2.v1_m128, _mm_set_ps1(ma1.v0.y))
//		),										
//		_mm_add_ps(								
//			_mm_mul_ps(ma2.v2_m128, _mm_set_ps1(ma1.v0.z)),
//			_mm_mul_ps(ma2.v3_m128, _mm_set_ps1(ma1.v0.w))
//		)
//	);
//	returnVal1.v1_m128 = _mm_add_ps(
//		_mm_add_ps(
//			_mm_mul_ps(ma2.v0_m128, _mm_set_ps1(ma1.v1.x)),
//			_mm_mul_ps(ma2.v1_m128, _mm_set_ps1(ma1.v1.y))
//		),										
//		_mm_add_ps(								
//			_mm_mul_ps(ma2.v2_m128, _mm_set_ps1(ma1.v1.z)),
//			_mm_mul_ps(ma2.v3_m128, _mm_set_ps1(ma1.v1.w))
//		)
//	);
//	returnVal1.v2_m128 = _mm_add_ps(
//		_mm_add_ps(
//			_mm_mul_ps(ma2.v0_m128, _mm_set_ps1(ma1.v2.x)),
//			_mm_mul_ps(ma2.v1_m128, _mm_set_ps1(ma1.v2.y))
//		),										
//		_mm_add_ps(								
//			_mm_mul_ps(ma2.v2_m128, _mm_set_ps1(ma1.v2.z)),
//			_mm_mul_ps(ma2.v3_m128, _mm_set_ps1(ma1.v2.w))
//		)
//	);
//	returnVal1.v3_m128 = _mm_add_ps(
//		_mm_add_ps(
//			_mm_mul_ps(ma2.v0_m128, _mm_set_ps1(ma1.v3.x)),
//			_mm_mul_ps(ma2.v1_m128, _mm_set_ps1(ma1.v3.y))
//		),										
//		_mm_add_ps(								
//			_mm_mul_ps(ma2.v2_m128, _mm_set_ps1(ma1.v3.z)),
//			_mm_mul_ps(ma2.v3_m128, _mm_set_ps1(ma1.v3.w))
//		)
//	);
//
//
//	// returnVal2 = returnVal2*m5
//	returnVal2.v0_m128 = _mm_add_ps(
//		_mm_add_ps(
//			_mm_mul_ps(ma5.v0_m128, _mm_set_ps1(returnVal2.v0.x)),
//			_mm_mul_ps(ma5.v1_m128, _mm_set_ps1(returnVal2.v0.y))
//		),										
//		_mm_add_ps(								
//			_mm_mul_ps(ma5.v2_m128, _mm_set_ps1(returnVal2.v0.z)),
//			_mm_mul_ps(ma5.v3_m128, _mm_set_ps1(returnVal2.v0.w))
//		)
//	);
//	returnVal2.v1_m128 = _mm_add_ps(
//		_mm_add_ps(
//			_mm_mul_ps(ma5.v0_m128, _mm_set_ps1(returnVal2.v1.x)),
//			_mm_mul_ps(ma5.v1_m128, _mm_set_ps1(returnVal2.v1.y))
//		),										
//		_mm_add_ps(								
//			_mm_mul_ps(ma5.v2_m128, _mm_set_ps1(returnVal2.v1.z)),
//			_mm_mul_ps(ma5.v3_m128, _mm_set_ps1(returnVal2.v1.w))
//		)
//	);
//	returnVal2.v2_m128 = _mm_add_ps(
//		_mm_add_ps(
//			_mm_mul_ps(ma5.v0_m128, _mm_set_ps1(returnVal2.v2.x)),
//			_mm_mul_ps(ma5.v1_m128, _mm_set_ps1(returnVal2.v2.y))
//		),										
//		_mm_add_ps(								
//			_mm_mul_ps(ma5.v2_m128, _mm_set_ps1(returnVal2.v2.z)),
//			_mm_mul_ps(ma5.v3_m128, _mm_set_ps1(returnVal2.v2.w))
//		)
//	);
//	returnVal2.v3_m128 = _mm_add_ps(
//		_mm_add_ps(
//			_mm_mul_ps(ma5.v0_m128, _mm_set_ps1(returnVal2.v3.x)),
//			_mm_mul_ps(ma5.v1_m128, _mm_set_ps1(returnVal2.v3.y))
//		),										
//		_mm_add_ps(								
//			_mm_mul_ps(ma5.v2_m128, _mm_set_ps1(returnVal2.v3.z)),
//			_mm_mul_ps(ma5.v3_m128, _mm_set_ps1(returnVal2.v3.w))
//		)
//	);
//
//
//	// returnVal1 = returnVal1*returnVal2
//	returnVal1.v0_m128 = _mm_add_ps(
//		_mm_add_ps(
//			_mm_mul_ps(returnVal2.v0_m128, _mm_set_ps1(returnVal1.v0.x)),
//			_mm_mul_ps(returnVal2.v1_m128, _mm_set_ps1(returnVal1.v0.y))
//		),											   
//		_mm_add_ps(									   
//			_mm_mul_ps(returnVal2.v2_m128, _mm_set_ps1(returnVal1.v0.z)),
//			_mm_mul_ps(returnVal2.v3_m128, _mm_set_ps1(returnVal1.v0.w))
//		)
//	);
//	returnVal1.v1_m128 = _mm_add_ps(
//		_mm_add_ps(
//			_mm_mul_ps(returnVal2.v0_m128, _mm_set_ps1(returnVal1.v1.x)),
//			_mm_mul_ps(returnVal2.v1_m128, _mm_set_ps1(returnVal1.v1.y))
//		),											   
//		_mm_add_ps(									   
//			_mm_mul_ps(returnVal2.v2_m128, _mm_set_ps1(returnVal1.v1.z)),
//			_mm_mul_ps(returnVal2.v3_m128, _mm_set_ps1(returnVal1.v1.w))
//		)
//	);
//	returnVal1.v2_m128 = _mm_add_ps(
//		_mm_add_ps(
//			_mm_mul_ps(returnVal2.v0_m128, _mm_set_ps1(returnVal1.v2.x)),
//			_mm_mul_ps(returnVal2.v1_m128, _mm_set_ps1(returnVal1.v2.y))
//		),											   
//		_mm_add_ps(									   
//			_mm_mul_ps(returnVal2.v2_m128, _mm_set_ps1(returnVal1.v2.z)),
//			_mm_mul_ps(returnVal2.v3_m128, _mm_set_ps1(returnVal1.v2.w))
//		)
//	);
//	returnVal1.v3_m128 = _mm_add_ps(
//		_mm_add_ps(
//			_mm_mul_ps(returnVal2.v0_m128, _mm_set_ps1(returnVal1.v3.x)),
//			_mm_mul_ps(returnVal2.v1_m128, _mm_set_ps1(returnVal1.v3.y))
//		),									
//		_mm_add_ps(									  
//			_mm_mul_ps(returnVal2.v2_m128, _mm_set_ps1(returnVal1.v3.z)),
//			_mm_mul_ps(returnVal2.v3_m128, _mm_set_ps1(returnVal1.v3.w))
//		)
//	);
//
//
//	// returnVal1 = ma1*ma2*ma3*ma4*ma5
//	return returnVal1;
//}

MmulMmulMmulMmulM::operator Matrix()
{
	// returnVal1 = m1*m2
	Matrix ma1ma2(ma1);
	ma1ma2.v3_m128 = ma2.v3_m128;
	Matrix returnVal1(ma1ma2);

	// returnVal2 = returnVal1*m3
	Matrix returnVal2;
	for (int i = 0; i < 4; i++)
	{
		((Vect4D*)&returnVal2 + i)->_m = _mm_add_ps(
			_mm_add_ps(
				_mm_mul_ps(ma3.v0_m128, _mm_set_ps1(((Vect4D*)&returnVal1 + i)->x)),
				_mm_mul_ps(ma3.v1_m128, _mm_set_ps1(((Vect4D*)&returnVal1 + i)->y))
			),
			_mm_add_ps(
				_mm_mul_ps(ma3.v2_m128, _mm_set_ps1(((Vect4D*)&returnVal1 + i)->z)),
				_mm_mul_ps(ma3.v3_m128, _mm_set_ps1(((Vect4D*)&returnVal1 + i)->w))
			)
		);
	}


	// returnVal1 = returnVal2*m4
	for (int i = 0; i < 4; i++)
	{
		((Vect4D*)&returnVal1 + i)->_m = _mm_add_ps(
			_mm_add_ps(
				_mm_mul_ps(ma4.v0_m128, _mm_set_ps1(((Vect4D*)&returnVal2 + i)->x)),
				_mm_mul_ps(ma4.v1_m128, _mm_set_ps1(((Vect4D*)&returnVal2 + i)->y))
			),
			_mm_add_ps(
				_mm_mul_ps(ma4.v2_m128, _mm_set_ps1(((Vect4D*)&returnVal2 + i)->z)),
				_mm_mul_ps(ma4.v3_m128, _mm_set_ps1(((Vect4D*)&returnVal2 + i)->w))
			)
		);
	}


	// returnVal2 = returnVal1*m4
	for (int i = 0; i < 4; i++)
	{
		((Vect4D*)&returnVal2 + i)->_m = _mm_add_ps(
			_mm_add_ps(
				_mm_mul_ps(ma5.v0_m128, _mm_set_ps1(((Vect4D*)&returnVal1 + i)->x)),
				_mm_mul_ps(ma5.v1_m128, _mm_set_ps1(((Vect4D*)&returnVal1 + i)->y))
			),
			_mm_add_ps(
				_mm_mul_ps(ma5.v2_m128, _mm_set_ps1(((Vect4D*)&returnVal1 + i)->z)),
				_mm_mul_ps(ma5.v3_m128, _mm_set_ps1(((Vect4D*)&returnVal1 + i)->w))
			)
		);
	}


	// returnVal2 = ma1*ma2*ma3*ma4*ma5
	return returnVal2;
}




// --- End of File ---
