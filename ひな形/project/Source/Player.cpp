#include "Player.h"
//#include"Field.h"

static const float Gravity = 0.2;
static const float V0 = -7.3;
Player::Player()
{
	hImage = LoadGraph("data/image/aoi.png");
	x = 200;
	y = 500;
	speed = 10;
}


Player::Player(int sx, int sy)
{
	hImage = LoadGraph("data/image/aoi.png");
	x = sx;
	y = sy;
	speed = 10;

}

Player::~Player()
{

}

void Player::Update()
{
	if (CheckHitKey(KEY_INPUT_D))
	{
		x += speed;
		if (x < 0) x = 0;
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		x -= speed;
		if (x > 640 - 64) x = 640 - 64;
	}
	if (CheckHitKey(KEY_INPUT_W))
	{
		y -= speed;
		if (y < 0) x = 0;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		y += speed;
	}


}

void Player::Draw()
{
	//Field* field = FindGameObject<Field>();
	DrawRectGraph(x , y, 0, 0, 64, 64, hImage, 1);
}
