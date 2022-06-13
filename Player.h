#pragma once

#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include <math.h>
#include <memory>
#include <list>

#include "Affine_trans.h"
#include "PlayerBullet.h"
class Player {
  public:
	//������
	void Initialize(Model* model, uint32_t textureHandle);

	//�X�V
	void Update();

	void Attack();

	//�`��
	void Draw(ViewProjection viewProjection);

  private:

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//�v���C���[�֘A
	//�A�t�B��
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//�X�s�[�h
	float character_speed_x = 0.2f;
	float character_speed_y = 0.2f;

	float rotation_speed_y = 0.2f;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

};
