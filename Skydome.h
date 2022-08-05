#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

class Skydome 
{
  public:
	// ������
	void Initialize(Model* model);

	// �X�V
	void Update();

	// �`��
	void Draw(ViewProjection viewProjection);

  private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	// ���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
};