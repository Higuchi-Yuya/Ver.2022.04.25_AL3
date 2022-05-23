#include "Matrix4.h"

Matrix4& Matrix4::operator*(const Matrix4& m2) 
{
	Matrix4 temp(
	  this->m[0][0], this->m[0][1], this->m[0][2], this->m[0][3], this->m[1][0], this->m[1][1],
	  this->m[1][2], this->m[1][3], this->m[2][0], this->m[2][1], this->m[2][2], this->m[2][3],
	  this->m[3][0], this->m[3][1], this->m[3][2], this->m[3][3]);

	temp *= m2;

	return temp;
}
