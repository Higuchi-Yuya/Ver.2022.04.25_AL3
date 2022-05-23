#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

const double Pi = 3.141593;

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete debugCamera_; 
	delete model_;
}

Vector3 GameScene::Get_Reference_point(Vector3& vertex) 
{ return vertex; }

double GameScene::Rad(double degree) 
{ 
	return degree * Pi / 180;
}

void GameScene::scale(Vector3& vertex, Vector3& reference_point, float expansion_rate_x, float expansion_rate_y, float expansion_rate_z) 
{
	float w = 0;
	//拡大縮小
	vertex -= reference_point;
	vertex.x = (vertex.x * expansion_rate_x) + (vertex.y * 0) + (0 * vertex.z) + w;
	vertex.y = (vertex.x * 0) + (vertex.y * expansion_rate_y) + (0 * vertex.z) + w;
	vertex.z = (vertex.x * 0) + (vertex.y * 0) + (vertex.z * expansion_rate_z) + w;
	vertex += reference_point;

}

void GameScene::translation(Vector3& vertex, float Tx, float Ty, float Tz) 
{
	float w = 1;
	//移動
	vertex.x = (vertex.x * 1) + (vertex.y * 0) + (vertex.z * 0) + (Tx * w);
	vertex.y = (vertex.x * 0) + (vertex.y * 1) + (vertex.z * 0) + (Ty * w);
	vertex.z = (vertex.x * 0) + (vertex.y * 0) + (vertex.z * 1) + (Tz * w);
}

void GameScene::rotate(Vector3& vertex, Vector3& reference_point, float frequency_x, float frequency_y, float frequency_z) 
{
	float w = 0;
	float h_x, h_y, h_z;
	
	//xの回転
	vertex -= reference_point;
	h_x = vertex.x, h_y = vertex.y, h_z = vertex.z;
	vertex.x = (h_x * 1) + (h_y * 0) + (h_z * 0) + (0 * w);
	vertex.y = (h_x * 0) + (h_y * cos(Rad(frequency_x))) + (h_z * sin(Rad(frequency_x))) + (0 * w);
	vertex.z = (h_x * 0) + (h_y * -sin(Rad(frequency_x))) + (h_z * cos(Rad(frequency_x))) + (0 * w);
	vertex += reference_point;

	//yの回転
	vertex -= reference_point;
	h_x = vertex.x, h_y = vertex.y, h_z = vertex.z;
	vertex.x = (h_x * cos(Rad(frequency_y))) + (h_y * 0) + (h_z * -sin(Rad(frequency_y))) + (0 * w);
	vertex.y = (h_x * 0) + (h_y * 1) + (h_z * 0) + (0 * w);
	vertex.z = (h_x * sin(Rad(frequency_y))) + (h_y * 0) + (h_z * cos(Rad(frequency_y))) + (0 * w);
	vertex += reference_point;

	//zの回転
	vertex -= reference_point;
	h_x = vertex.x, h_y = vertex.y, h_z = vertex.z;
	vertex.x = (h_x * cos(Rad(frequency_z))) + (h_y * sin(Rad(frequency_z))) + (h_z * 0) + (0 * w);
	vertex.y = (h_x * -sin(Rad(frequency_z))) + (h_y * cos(Rad(frequency_z))) + (h_z * 0) + (0 * w);
	vertex.z = (h_x * 0) + (h_y * 0) + (h_z * 1) + (0 * w);
	vertex += reference_point;

	
}


void GameScene::Initialize() {

	textureHandle_ = TextureManager::Load("mario.jpg");

	debugCamera_ = new DebugCamera(1200, 720);

	Affine_trans trans;

	//３Dモデルの生成
	model_ = Model::Create();
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());


	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	


	for (int i = 0; i < 8; i++) {
		scale(box_vecter[i], box_vecter[0], 2, 3, 4);
		translation(box_vecter3[i], 10, 10, 10);
		rotate(box_vecter4[i], box_vecter[0], 45, 45, 45);
	
	}

	//グリット線の描画位置設定
	
	for (int i = 0; i < 9; i++) {
		translation(grit_line_x_s[i], grit_x, 0, -20);
		grit_x += 5;
	}
	grit_x = -20;
	for (int i = 0; i < 9; i++) {
		translation(grit_line_x_e[i], grit_x, 0, 20);
		grit_x += 5;
	}

	for (int i = 0; i < 9; i++) {
		translation(grit_line_z_s[i], -20, 0, grit_z);
		grit_z += 5;
	}
	grit_z = -20;
	for (int i = 0; i < 9; i++) {
		translation(grit_line_z_e[i], 20, 0, grit_z);
		grit_z += 5;
	}

	//X,Y,Z方向のスケーリングを設定
	worldTransform_.scale_ = {Sx = 5, Sy = 1, Sz = 5};

	//スケーリング行列を宣言
	Matrix4 matScale;

	trans.scale(matScale, Sx, Sy, Sz);


	// X,Y,Z方向の回転を設定
	worldTransform_.rotation_ = {Rx = 45.0f, Ry = 45.0f, Rz = 45.0f};

	// 合成用回転行列を宣言
	Matrix4 matRot;
	Matrix4 matRotX, matRotY, matRotZ;
	
	// 各自軸用回転行列を宣言
	trans.rotateX(matRotX, Rx);
	trans.rotateY(matRotY, Ry);
	trans.rotateZ(matRotZ, Rz);
	
	//回転行列の単位行列
	trans.identity_matrix(matRot);

	// 各軸の回転行列を合成
	matRot = matRotX * matRotY * matRotZ;

	// X,Y,Z軸周りの平行移動を設定
	worldTransform_.translation_ = {Tx = 10, Ty = 0, Tz = 0};

	// 平行移動行列を宣言
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	trans.translation(matTrans, Tx, Ty, Tz);

	//単位行列を入れる
	trans.identity_matrix(worldTransform_.matWorld_);
	
	worldTransform_.matWorld_ *= matScale;
	worldTransform_.matWorld_ *= matRot;
	worldTransform_.matWorld_ *= matTrans;
	
	//行列の転送
	worldTransform_.TransferMatrix();


	//ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
	
}

void GameScene::Update() { 
	debugCamera_->Update();
	
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	//ライン描画が参照するビュープロジェクションを指定する
	
	//ボックスの描画
	/*for (int i=0;i<12;i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d( box_vecter[vertex[i][0]], box_vecter[vertex[i][1]], box_vecter_);
		PrimitiveDrawer::GetInstance()->DrawLine3d( box_vecter2[vertex[i][0]], box_vecter2[vertex[i][1]], box_vecter2_);
		PrimitiveDrawer::GetInstance()->DrawLine3d( box_vecter3[vertex[i][0]], box_vecter3[vertex[i][1]], box_vecter3_);
		PrimitiveDrawer::GetInstance()->DrawLine3d( box_vecter4[vertex[i][0]], box_vecter4[vertex[i][1]], box_vecter4_);
	}*/

	//グリット線の描画
	for (int i = 0; i < 9; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(grit_line_x_s[i], grit_line_x_e[i], grit_color_x);
		PrimitiveDrawer::GetInstance()->DrawLine3d(grit_line_z_s[i], grit_line_z_e[i], grit_color_z);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}




