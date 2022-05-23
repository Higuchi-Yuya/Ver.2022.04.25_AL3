#include "Affine_trans.h"

double Pi = 3.14159265;

Affine_trans::Affine_trans() {}

Affine_trans::~Affine_trans() {}

double Affine_trans::Rad(double degree) 
{ 
	return degree * Pi / 180; }

void Affine_trans::identity_matrix(Matrix4& vertex) 
{
	vertex.m[0][0] = 1;
	vertex.m[1][1] = 1;
	vertex.m[2][2] = 1;
	vertex.m[3][3] = 1;
}



void Affine_trans::scale(Matrix4& vertex, float expansion_rate_x, float expansion_rate_y, float expansion_rate_z) 
{
	vertex.m[0][0] = expansion_rate_x;
	vertex.m[1][1] = expansion_rate_y;
	vertex.m[2][2] = expansion_rate_z;
	vertex.m[3][3] = 1;
}

void Affine_trans::translation(Matrix4& vertex, float Tx, float Ty, float Tz) 
{
	identity_matrix(vertex);
	vertex.m[3][0] = Tx;
	vertex.m[3][1] = Ty;
	vertex.m[3][2] = Tz;
}

void Affine_trans::rotateX(Matrix4& vertex, float frequency_x) {
	// X�̉�]
	vertex.m[0][0] = 1;
	vertex.m[1][1] = cos(Rad(frequency_x));
	vertex.m[1][2] = sin(Rad(frequency_x));
	vertex.m[2][1] = -sin(Rad(frequency_x));
	vertex.m[2][2] = cos(Rad(frequency_x));
	vertex.m[3][3] = 1;
}

void Affine_trans::rotateY(Matrix4& vertex, float frequency_y) {
	// Y�̉�]
	vertex.m[0][0] = cos(Rad(frequency_y));
	vertex.m[0][2] = -sin(Rad(frequency_y));
	vertex.m[1][1] = 1;
	vertex.m[2][0] = sin(Rad(frequency_y));
	vertex.m[2][2] = cos(Rad(frequency_y));
	vertex.m[3][3] = 1;
}

void Affine_trans::rotateZ(Matrix4& vertex, float frequency_z) {
	// Z�̉�]
	vertex.m[0][0] = cos(Rad(frequency_z));
	vertex.m[0][1] = sin(Rad(frequency_z));
	vertex.m[1][0] = -sin(Rad(frequency_z));
	vertex.m[1][1] = cos(Rad(frequency_z));
	vertex.m[2][2] = 1;
	vertex.m[3][3] = 1;
}

void Affine_trans::rotate(Matrix4& vertex, float frequency_x, float frequency_y, float frequency_z) 
{
	Matrix4 RotX, RotY, RotZ;



	

	//vertex = RotZ * RotX * RotY;

}

