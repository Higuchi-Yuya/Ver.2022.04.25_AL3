#include "RailCamera.h"
#include "Affine_trans.h"


void RailCamera::Initialize(WorldTransform worldTransform) {
	// ���[���h�g�����X�t�H�[���̃f�[�^����
	/*worldTransform_.Initialize();*/
	/*worldTransform_.translation_ = worldTransform.translation_;
	worldTransform_.rotation_ = worldTransform.rotation_;*/
	worldTransform_ = worldTransform;
	debugText_ = DebugText::GetInstance();


	viewProjection_.farZ = 100.0f;
	// �r���[�v���W�F�N�V�����̏�����
	viewProjection_.Initialize();
}

void RailCamera::Update() { 
#pragma region ���[���h�s��̏���
	Affine_trans* trans = nullptr;
	Vector3 move = {0, 0, 0};
	Vector3 rotate = {0, 0, 0};

	// �J�����I�u�W�F�̈ړ�����
	move = {0.0f, 0.0f, -moveSpeed_.z};
	//worldTransform_.translation_ += move;

	// �J�����I�u�W�F�̉�]����
	rotate = {0.0f, +rotateSpeed_.y, 0.0f};
	//worldTransform_.rotation_ += rotate;

	// ���[���h�s��̍Čv�Z
	trans->identity_matrix(worldTransform_.matWorld_);
	trans->Affine_Trans(worldTransform_.matWorld_, worldTransform_.scale_, worldTransform_.rotation_,worldTransform_.translation_);

	
#pragma endregion

#pragma region �r���[�v���W�F�N�V�����̏���
	viewProjection_.eye.x = worldTransform_.matWorld_.m[3][0];
	viewProjection_.eye.y = worldTransform_.matWorld_.m[3][1];
	viewProjection_.eye.z = worldTransform_.matWorld_.m[3][2];

	// ���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);

	// ���[���J�����̉�]�𔽉f
	trans->Vec3conversion_W_Notincluded(forward, worldTransform_.matWorld_);
	
	// ���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	viewProjection_.target = viewProjection_.eye + forward;
	
	// ���[���h����x�N�g��
	Vector3 up(0, 1, 0);

	// ���[���J�����̉�]�𔽉f�i���[���J�����̏���x�N�g���j
	trans->Vec3conversion_W_Notincluded(up,worldTransform_.matWorld_);
	viewProjection_.up = up;

	// �r���[�v���W�F�N�V�������X�V
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

#pragma endregion

#pragma region �f�o�b�O�e�L�X�g
	//�f�o�b�O�e�L�X�g�\��
	debugText_->SetPos(50, 180);
	debugText_->Printf(
	  "Eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	debugText_->SetPos(50, 200);
	debugText_->Printf(
	  "Target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,viewProjection_.target.z);
	debugText_->SetPos(50, 220);
	debugText_->Printf("Forward:(%f,%f,%f)", forward.x, forward.y, forward.z);
	debugText_->SetPos(50, 240);
	debugText_->Printf("Up:(%f,%f,%f)", up.x, up.y, up.z);
#pragma endregion
}

ViewProjection RailCamera::GetViewProjection() { return viewProjection_; }

WorldTransform* RailCamera::GetWorldTransform() { return &worldTransform_; }


