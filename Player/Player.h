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
	~Player();

	//������
	void Initialize(Model* model, uint32_t textureHandle);

	//�X�V
	void Update();

	void Attack();

	//�`��
	void Draw(ViewProjection viewProjection);

	// ���[���h���W���擾
	Vector3 GetWorldPosition();

	// �Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	// �e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	void SetWorldTransformParent(WorldTransform *worldtransform);

	// 3D���e�B�N���̍X�V����
	void ReticleUpdate(ViewProjection viewProjection);

	/// <summary>
	/// UI�`��
	/// </summary>
	void DrawUI();

  private:
	//�ړ�����
	void Move();

	void Rotation();

	//�s��ϊ�
	void Trans_Update();

	//�f�o�b�O�e�L�X�g
	void Debug_Text();



	// 3D���e�B�N���̕`�揈��
	void ReticleDraw(ViewProjection viewProjection);

	

//�����o�ϐ�
  private:
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	WorldTransform cameraworld_;

	// 3D���e�B�N���p���[���h�g�����X�t�H�[��
	WorldTransform worldTransform3DReticle_;

	//���f��
	Model* model_ = nullptr;

	//�v���C���[�֘A
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//�X�s�[�h
	float character_speed_x = 0.2f;
	float character_speed_y = 0.2f;

	float rotation_speed_y = 0.01f;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	// 2D���e�B�N���p�X�v���C�g
	std::unique_ptr<Sprite> sprite2DReticle_;
	
};
