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

class Player;

class GameScene;

class Enemy
{
  public:
	//�G�̊�{�I�Ȋ֐�
	void Initialize(Model* model, Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	//�ڋ߃t�F�[�Y�֘A�̊֐�
	void Approach_Initialize();
	void Approach_Update();

	//���E�t�F�[�Y�֘A�̊֐�
	void Laeve_Update();

	// �G�L�����Ɏ��L�����̃A�h���X��n��
	void SetPlayer(Player* player) { player_ = player; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	//�e�̔��ˊ֐�
	void Fire();

	// ���[���h���W���擾����
	Vector3 GetWorldPosition();

	// �Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	// �e���X�g���擾
	//const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	bool IsDead() const { return isDead_; }

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
	float enemy_speed_z = 0.05f;
	
	//���˃^�C�}�[
	int32_t fireTimer_ = 60;

	//�A�t�B���p
	Affine_trans* trans = nullptr;

	//�f�X�t���O
	bool isDead_ = false;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

	// ���L����
	Player* player_ = nullptr;

	// �Q�[���V�[��
	GameScene* gameScene_ = nullptr;
};
