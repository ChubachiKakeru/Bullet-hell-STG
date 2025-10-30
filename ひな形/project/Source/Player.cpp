#include "Player.h"
#include"Field.h"
#include"Bullet.h"
#include "DxLib.h"

//static const float Gravity = 0.2;
//static const float V0 = -7.3;
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


	// ��ړ� (W�L�[) - �V�K�ǉ�
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

	// ���ړ� (S�L�[) - �V�K�ǉ�
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

	// �e���˃^�C�}�[�X�V
	shotTimer += 1.0f;

	// H�L�[�Œe����
	if (CheckHitKey(KEY_INPUT_H) && shotTimer >= 10.0f) {
		ShootBullet();
		shotTimer = 0.0f;  // �^�C�}�[���Z�b�g
	}
	/*if (onGround == true)
		{
			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				velocity = V0;
				onGround = false;
			}
		}

		y += velocity;
		velocity += Gravity;
		if (velocity >= 0)
		{
			Field* field = FindGameObject<Field>();
			int push1 = field->HitCheckDown(x + 14, y + 64);
			int push2 = field->HitCheckDown(x + 50, y + 64);
			int push = max(push1, push2);
			if (push > 0)
			{
				y -= push - 1;
				velocity = 0;
				onGround = true;
			}
			else
			{
				onGround = false;
			}
		}
		else
		{
			Field* field = FindGameObject<Field>();
			int push1 = field->HitCheckUp(x + 14, y + 5);
			int push2 = field->HitCheckUp(x + 50, y + 5);
			int push = max(push1, push2);
			if (push > 0)
			{
				y += push;
				velocity = 0;
			}
		}*/
	
}

void Player::ShootBullet()
{
	// �v���C���[�̒��S����e�𔭎�
	new Bullet((int)x + 320/2, (int)y + 320/2, 0, -10.0f);
}

void Player::Draw()
{
	Field* field = FindGameObject<Field>();
	DrawGraph(x , y, hImage, 1);
}


