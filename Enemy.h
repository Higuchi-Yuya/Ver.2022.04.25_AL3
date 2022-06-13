#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>

#include "Affine_trans.h"
#include "EnemyBullet.h"
enum class Phase {
	Approach, //�ڋ߂���
	Laeve,    //���E����

};

class Enemy 
{
  public:
	//�G�̊�{�I�Ȋ֐�
	void Initialize(Model* model);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	//�ڋ߃t�F�[�Y�֘A�̊֐�
	void Approach_Initialize();
	void Approach_Update();

	//���E�t�F�[�Y�֘A�̊֐�
	void Laeve_Update();

	//�e�̔��ˊ֐�
	void Fire();

	
  public:
	//���ˊԊu
	static const int kFireInterval = 60;
  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	float enemy_speed_x = 0.1f;
	float enemy_speed_y = 0.1f;
	float enemy_speed_z = 0.1f;
	
	//���˃^�C�}�[
	int32_t fireTimer_ = 0;

	//�A�t�B���p
	Affine_trans* trans = nullptr;

	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;
};
