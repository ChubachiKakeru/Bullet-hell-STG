#include "Player.h"
#include"Field.h"
#include"Bullet.h"
#include "DxLib.h"

Player::Player() : GameObject()
{
	hImage = LoadGraph("data/image/file/chara/player.png");
	x = 200;
	y = 500;
	velocity = 0;
	onGround = false;
	shotTimer = 0.0f;
}



Player::Player(int sx, int sy)
{
	hImage = LoadGraph("data/image/file/chara/player.png");
	x = sx;
	y = sy;
	velocity = 0;
	onGround = false;
	shotTimer = 0.0f;
}

Player::~Player()
{
	if (hImage != -1) {
		DeleteGraph(hImage);
	}
}

void Player::Initialize(float startX, float startY, Field* fieldPtr)
{
	hImage = LoadGraph("data/image/file/chara/player.png");
	x = startX;
	y = startY;
	velocity = 0;
	onGround = false;
	field = fieldPtr;
}

void Player::Update()
{
	float nextX = x;
	float nextY = y;

	if (CheckHitKey(KEY_INPUT_D))
	{
		x += 5;
		Field* field = FindGameObject<Field>();
		int push1 = field->HitCheckRight(x + 50, y + 5);
		int push2 = field->HitCheckRight(x + 50, y + 63);
		x -= max(push1, push2);
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		x -= 5;
		Field* field = FindGameObject<Field>();
		int push1 = field->HitCheckLeft(x + 14, y + 5);
		int push2 = field->HitCheckLeft(x + 14, y + 63);
		x -= max(push1, push2);
	}


	// 上移動 (Wキー) - 新規追加
	if (CheckHitKey(KEY_INPUT_W)) {
		y -= 5;
		Field* field = FindGameObject<Field>();
		int push1 = field->HitCheckUp(x + 14, y + 5);
		int push2 = field->HitCheckUp(x + 50, y + 5);
		int push = max(push1, push2);
		if (push > 0) {
			y += push;
			velocity = 0;
		}
	}

	// 下移動 (Sキー) - 新規追加
	if (CheckHitKey(KEY_INPUT_S)) {
		y += 5;
		Field* field = FindGameObject<Field>();
		int push1 = field->HitCheckDown(x + 14, y + 64);
		int push2 = field->HitCheckDown(x + 50, y + 64);
		int push = max(push1, push2);
		if (push > 0) {
			y -= push - 1;
			velocity = 0;
			onGround = false;
		}
	}	

	// 弾発射タイマー更新
	shotTimer += 1.0f;

	// Hキーで弾発射
	if (CheckHitKey(KEY_INPUT_H) && shotTimer >= 10.0f) {
		ShootBullet();
		shotTimer = 0.0f;  // タイマーリセット
	}
	
}

void Player::ShootBullet()
{
	// プレイヤーの中心から弾を発射
	new Bullet((int)x + 50/2, (int)y + 50/2, 0, -10.0f);
}

void Player::Draw()
{
	Field* field = FindGameObject<Field>();
	DrawGraph(x , y, hImage, 1);
}


