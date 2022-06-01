#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#include <iostream>
#include <algorithm>



const float Pi = 3.141593f;

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
	for (WorldTransform& worldTransform_ : worldTransform_) {
		worldTransform_.Initialize();
	}


	//カメラ視点座標を設定
	viewProjection_.eye = {Eye_x, Eye_y, Eye_z};

	////カメラ注視点座標を設定
	//viewProjection_.target = {Target_x, Target_y, Target_z};
	//
	////カメラ上方向ベクトルを設定 (右上45度指定)
	//viewProjection_.up = {cosf(Pi / 4.0f), sinf(Pi / 4.0f), 0.0f};

	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = 50.0f * (Pi / 180);

	//アスペクト比を設定
	viewProjection_.aspectRatio = 1.0f;

	//ニアクリップ距離を設定
	viewProjection_.nearZ = 52.0f;

	//ファークリップ距離を設定
	viewProjection_.farZ = 53.0f;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);


	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//乱数範囲の指定
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);
	
	std::uniform_real_distribution<float> rotDist(0.0f, 360.0f);
	

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	
	for (int i = 0; i < 8; i++) {
		scale(box_vecter[i], box_vecter[0], 2, 3, 4);
		translation(box_vecter3[i], 10, 10, 10);
		rotate(box_vecter4[i], box_vecter[0], 45, 45, 45);
	
	}

#pragma region グリット線の描画の設定
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
#pragma endregion


#pragma region スケーリング行列の設定
	// X,Y,Z方向のスケーリングを設定

	for (WorldTransform& worldTransform_ : worldTransform_) {
		worldTransform_.scale_ = {Sx = 1, Sy = 1, Sz = 1};
	}

	//スケーリング行列を宣言
	Matrix4 matScale;

	trans.scale(matScale, Sx, Sy, Sz);
#pragma endregion


#pragma region 回転行列の設定
	for (float& Rx : Rx) {
		Rx = rotDist(engine);
	}
	for (float& Ry : Ry) {
		Ry = rotDist(engine);
	}
	for (float& Rz : Rz) {
		Rz = rotDist(engine);
	}
	// X,Y,Z方向の回転を設定
	for (int i = 0; i < _countof(worldTransform_); i++) {
		
		worldTransform_[i].rotation_ = {Rx[i], Ry[i], Rz[i]};
	}
	

	// 合成用回転行列を宣言
	Matrix4 matRot[100];
	
	for (int i = 0; i < _countof(worldTransform_); i++) {
		// 各軸の回転行列を合成
		trans.rotate(matRot[i], Rx[i], Ry[i], Rz[i]);
	}
	

#pragma endregion

	
#pragma region 平行移動を設定
	for (float& Tx : Tx) {
		Tx = posDist(engine);
	}
	for (float& Ty : Ty) {
		Ty = posDist(engine);
	}
	for (float& Tz : Tz) {
		Tz = posDist(engine);
	}
	for (int i = 0; i < _countof(worldTransform_); i++) {
		worldTransform_[i].translation_ = {Tx[i], Ty[i], Tz[i]};
	}

	// 平行移動行列を宣言
	Matrix4 matTrans[100];
	for (int i = 0; i < _countof(worldTransform_); i++) {
		matTrans[i] = MathUtility::Matrix4Identity();
		trans.translation(matTrans[i], Tx[i], Ty[i], Tz[i]);
	}
	

#pragma endregion

	

	//単位行列を入れる
	for (int i = 0; i < _countof(worldTransform_);i++) {

		trans.identity_matrix(worldTransform_[i].matWorld_);

		worldTransform_[i].matWorld_ *= matScale;
		worldTransform_[i].matWorld_ *= matRot[i];
		worldTransform_[i].matWorld_ *= matTrans[i];

		//行列の転送
		worldTransform_[i].TransferMatrix();
	}
	


	//ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
	
}

void GameScene::Update() { 
//#pragma region 視点の移動ベクトル
//	//視点移動ベクトル
//	Vector3 move(0, 0, 0);
//
//	//視点の移動速さ
//	const float kEyeSpeed = 0.2f;
//
//	//注視点の移動速さ
//	const float kTargetSpeed = 0.2f;
//
//	//押した方向で移動ベクトルを変更
//
//	//前後
//	if (input_->PushKey(DIK_W)) {
//		move.z += kEyeSpeed;
//	} else if (input_->PushKey(DIK_S)) {
//		move.z -= kEyeSpeed;
//	}
//
//	//左右
//	if (input_->PushKey(DIK_LEFT)) {
//		move.x += kTargetSpeed;
//	} else if (input_->PushKey(DIK_RIGHT)) {
//		move.x -= kTargetSpeed;
//	}
//	
//
//	//視点移動(ベクトルの加算)
//	viewProjection_.eye += move;
//
//	//注視点移動(ベクトル加算)
//	viewProjection_.target += move;
//
//	//行列の再計算
//	viewProjection_.UpdateMatrix();
//
//	//デバッグ用表示
//
	//視点
	debugText_->SetPos(50, 50);
	debugText_->Printf("eye:(%f,%f,%f)", 
						viewProjection_.eye.x, 
						viewProjection_.eye.y, 
						viewProjection_.eye.z);
//
//	//注視点
//	debugText_->SetPos(50, 70);
//	debugText_->Printf("target:(%f,%f,%f)", 
//						viewProjection_.target.x, 
//						viewProjection_.target.y,
//						viewProjection_.target.z);
//
//#pragma endregion
//
//
//#pragma region 上方向回転処理
//    //上方向の回転速さ[ラジアン/frame]
//	const float kUpRotSpeed = 0.05f;
//
//	//押した方向で移動ベクトルを変更
//	if (input_->PushKey(DIK_SPACE)) {
//		viewAngle += kUpRotSpeed;
//		//２πを超えたら０に戻す
//		viewAngle = fmodf(viewAngle, Pi * 2.0f);
//	}
//
//	//上方向ベクトルを計算(半径１の円周上の座標)
//	viewProjection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};
//
//	//行列の再計算
//	viewProjection_.UpdateMatrix();
//
//	//デバッグ用表示
//
//	//回転
//	debugText_->SetPos(50, 90);
//	debugText_->Printf("up:(%f,%f,%f)", 
//						viewProjection_.up.x,
//						viewProjection_.up.y, 
//						viewProjection_.up.z);
//
//#pragma endregion
	if (input_->PushKey(DIK_UP)) {
		//viewProjection_.fovAngleY += 0.01;
		viewProjection_.nearZ += 0.1;
	} else if (input_->PushKey(DIK_DOWN)) {
		//viewProjection_.fovAngleY -= 0.01;
		viewProjection_.nearZ -= 0.1;
	}
	viewProjection_.fovAngleY = std::clamp(viewProjection_.fovAngleY, 0.01f, Pi);
	
	viewProjection_.UpdateMatrix();

	debugText_->SetPos(50, 110);
	
	
	debugText_->Printf("fovAngleY(Degree):%f", viewProjection_.fovAngleY * (180 / Pi));
	debugText_->SetPos(50, 130);
	debugText_->Printf("nearZ:&f", viewProjection_.nearZ);
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
	for (WorldTransform& worldTransform_ : worldTransform_) {
		model_->Draw(worldTransform_,viewProjection_, textureHandle_);
	}
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




