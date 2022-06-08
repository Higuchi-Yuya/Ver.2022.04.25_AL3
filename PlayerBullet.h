#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

#include "Affine_trans.h"
class PlayerBullet 
{
  public:
	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//�A�t�B���p
	Affine_trans* trans = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
};
