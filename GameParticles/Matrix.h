//---------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//---------------------------------------------------------------

#ifndef MATRIX_H
#define MATRIX_H

// includes
#include "Enum.h"
#include "Vect4D.h"

// class
class Matrix
{
public:

	// local enumerations
	enum class MatrixRow
	{
		MATRIX_ROW_0,
		MATRIX_ROW_1,
		MATRIX_ROW_2,
		MATRIX_ROW_3
	};

	Matrix();	
	~Matrix();
	Matrix(const Matrix& t) = default;
	Matrix& operator=(const Matrix&) = default;

	Matrix(
		const float _m0, const float _m1, const float _m2, const float _m3,
		const float _m4, const float _m5, const float _m6, const float _m7,
		const float _m8, const float _m9, const float _m10, const float _m11,
		const float _m12, const float _m13, const float _m14, const float _m15
	);

	void set(const MatrixRow row, Vect4D *t );
	void get(const MatrixRow row, Vect4D *vOut );

	void setTransMatrix(const Vect4D *t );
	void setScaleMatrix(const Vect4D *s );
	void setRotZMatrix(const float Z_Radians );

	float &operator[]( Index e);
	
	Matrix operator*(const Matrix &t ) const;
	Matrix operator*(const float s );

	float Determinant();
	
	Matrix GetAdjugate();
	Matrix& operator/=(const float t);
	
	void Inverse( Matrix &out );
	
private:
	union
	{
		struct
		{
			Vect4D v0;
			Vect4D v1;
			Vect4D v2;
			Vect4D v3;
		};

		struct
		{
			// ROW 0
			float m0;
			float m1;
			float m2;
			float m3;

			// ROW 1
			float m4;
			float m5;
			float m6;
			float m7;

			// ROW 2
			float m8;
			float m9;
			float m10;
			float m11;

			// ROW 3
			float m12;
			float m13;
			float m14;
			float m15;
		};
	};
};

#endif  

// --- End of File ---
