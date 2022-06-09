#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

#include "Affine_trans.h"
class Enemy 
{
  public:
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);
  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	float enemy_speed_z = 0.1;

	//�A�t�B���p
	Affine_trans* trans = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
};
