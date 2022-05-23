#pragma once
#include "GameScene.h"
#include <math.h>

class Affine_trans {
  public:
	Affine_trans();
	~Affine_trans();

	double Rad(double degree);

	void identity_matrix(Matrix4& vertex);

	void scale(Matrix4& vertex, float expansion_rate_x, float expansion_rate_y,float expansion_rate_z);

	void translation(Matrix4& vertex, float Tx, float Ty, float Tz);

	void rotate(Matrix4& vertex, float frequency_x, float frequency_y,float frequency_z);

	void rotateX(Matrix4& vertex, float frequency_x);

	void rotateY(Matrix4& vertex, float frequency_y);

	void rotateZ(Matrix4& vertex, float frequency_z);
  private:
};
