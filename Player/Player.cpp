#include "Player.h"
#include "RailCamera.h"

//RailCamera* railCamera = nullptr;
float VpWidth = 1280;
float VpHeight = 720;

Player::~Player() 
{
	
}

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	// ���e�B�N���p�e�N�X�`���擾
	uint32_t textureReticle_ = TextureManager::Load("2DReticle_220.png");

	// �X�v���C�g����
	Vector2 pos = {10, 0};
	Vector4 color = {1, 1, 1, 1};
	Vector2 anker = {0.5f, 0.5f};
	sprite2DReticle_.reset(Sprite::Create(textureReticle_, pos, color, anker));


	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ���������
	worldTransform_.translation_.z = 50.0f;
	worldTransform_.Initialize();
	
	// 3D���e�B�N���̃��[���h�g�����X�t�H�[��������
	worldTransform3DReticle_.Initialize();


	
}

void Player::Update() {

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { return bullet->IsDead(); });

	//��]����
	Rotation();

	//�ړ�����
	Move();

	//�L�����N�^�[�̍U������
	Attack();

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}


	//�s��̍X�V����ѓ]��
	Trans_Update();



	//�f�o�b�O�e�L�X�g
	Debug_Text();
}

void Player::Attack() {

	if (input_->PushKey(DIK_Y)) {

		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity;
		Affine_trans trans;

		Vector3 player_pos = GetWorldPosition();
		Vector3 reticle_pos = {
		  worldTransform3DReticle_.matWorld_.m[3][0], worldTransform3DReticle_.matWorld_.m[3][1],
		  worldTransform3DReticle_.matWorld_.m[3][2]};

		// ���@����Ə��I�u�W�F�N�g�ւ̃x�N�g��
		velocity = reticle_pos - player_pos;
		velocity.normalize();
		velocity *= kBulletSpeed;

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		//velocity = velocity * worldTransform_.matWorld_;

		

		//�e�𐶐����A������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_, velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
}

void Player::Draw(ViewProjection viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 3D���e�B�N����`��
	ReticleDraw(viewProjection);

	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

Vector3 Player::GetWorldPosition() 
{
	// ���[���h���W������ϐ�
	Vector3 worldPos;
	// ���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision() {}

void Player::SetWorldTransformParent(WorldTransform *worldtransform) 
{
	worldTransform_.parent_ = worldtransform;
}

void Player::Move() 
{
	Vector3 move = {0, 0, 0};
	constexpr float MoveLimitX = 35;
	constexpr float MoveLimitY = 19;

	//�L�����N�^�[�̈ړ�����
	if (input_->PushKey(DIK_LEFT)) {
		move = {-character_speed_x, 0, 0};

		worldTransform_.translation_.x += move.x;

	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {character_speed_x, 0, 0};

		worldTransform_.translation_.x += move.x;

	} else if (input_->PushKey(DIK_UP)) {
		move = {0, character_speed_y, 0};

		worldTransform_.translation_.y += move.y;
	} else if (input_->PushKey(DIK_DOWN)) {
		move = {0, -character_speed_y, 0};

		worldTransform_.translation_.y += move.y;
	}

	// �͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -MoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +MoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -MoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +MoveLimitY);
}

void Player::Rotation() 
{ 
	Vector3 rotate = {0, 0, 0};

	//�L�����N�^�[�̐���ړ�
	if (input_->PushKey(DIK_U)) {
		rotate = {0, -rotation_speed_y, 0};
		worldTransform_.rotation_.y += rotate.y;
	}
	if (input_->PushKey(DIK_I)) {
		rotate = {0, +rotation_speed_y, 0};
		worldTransform_.rotation_.y += rotate.y;
	}
}

void Player::Trans_Update() 
{
	Affine_trans* trans = nullptr;
	
	//�x�N�g���̉��Z
	trans->identity_matrix(worldTransform_.matWorld_);

	

	trans->Affine_Trans(
	  worldTransform_.matWorld_, worldTransform_.scale_, worldTransform_.rotation_,
	  worldTransform_.translation_);

	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;
	//worldTransform_.matWorld_ *= railCamera->GetWorldTransform().matWorld_;
	//worldTransform_.matWorld_.m[3][2] += 50.0f;
	//�s��̓]��
	worldTransform_.TransferMatrix();
}

void Player::Debug_Text() 
{
	//�f�o�b�O�e�L�X�g�\��
	debugText_->SetPos(50, 150);
	debugText_->Printf(
	  "Pos:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
}

void Player::ReticleUpdate(ViewProjection viewProjection) 
{
	Affine_trans trans;
	// ���@�̃��[���h���W����3D���e�B�N���̃��[���h���W���v�Z
	{
		// ���@����3D���e�B�N���ւ̋���
		const float kDistancePlayerTo3DReticle = 40.0f;

		// ���@����3D���e�B�N���ւ̃I�t�Z�b�g(Z+����)
		Vector3 offset = {0, 0, 1.0f};

		// ���@�̃��[���h�s��̉�]�𔽉f
		trans.Vec3conversion_W_Notincluded(offset, worldTransform_.matWorld_);

		// �x�N�g���̒����𐮂���
		offset.normalize() *= kDistancePlayerTo3DReticle;

		// 3D���e�B�N���̍��W��ݒ�
		Vector3 player_pos = GetWorldPosition();

		worldTransform3DReticle_.translation_ = offset + player_pos;

		// ���[���h�s��̍X�V�Ɠ]��
		trans.Affine_Trans(
		  worldTransform3DReticle_.matWorld_, 
		  worldTransform3DReticle_.scale_,
		  worldTransform3DReticle_.rotation_, 
		  worldTransform3DReticle_.translation_);

		worldTransform3DReticle_.TransferMatrix();
	}

	// 3D���e�B�N���̃��[���h���W����2D���e�B�N���̃X�N���[�����W���v�Z
	{
		Vector3 positionReticle = {
		  worldTransform3DReticle_.matWorld_.m[3][0], worldTransform3DReticle_.matWorld_.m[3][1],
		  worldTransform3DReticle_.matWorld_.m[3][2]};

		// �r���[�|�[�g�s��
		Matrix4 matViewport =
		{
		 VpWidth/2, 0.0f       , 0.0f, 0.0f,
		 0.0f     , -VpHeight/2, 0.0f, 0.0f,
		 0.0f     , 0.0f       , 1.0f, 0.0f,
		 VpWidth/2, VpHeight/2 , 0.0f, 1.0f
		};

		// �r���[�s��ƃv���W�F�N�V�����s��A�r���[�|�[�g�s�����������
		Matrix4 matViewProjectionViewport =
		  viewProjection.matView * viewProjection.matProjection * matViewport;

		// ���[���h���X�N���[�����W�ϊ��i������3D����2D�ɂȂ�j
		trans.Vec3conversion_W_Included(positionReticle, matViewProjectionViewport);

		// �X�v���C�g�̃��e�B�N���ɍ��W�ݒ�
		sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
	}
}

void Player::ReticleDraw(ViewProjection viewProjection) 
{
	//model_->Draw(worldTransform3DReticle_, viewProjection);
}

void Player::DrawUI() 
{ 
	sprite2DReticle_.get()->Draw(); 
}
