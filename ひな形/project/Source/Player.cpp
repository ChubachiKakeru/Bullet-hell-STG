#include "Player.h"
#include"Field.h"

static const float Gravity = 0.2;
static const float V0 = -7.3;
Player::Player()
{
	hImage = LoadGraph("data/image/chara.png");
	x = 200;
	y = 500;
	velocity = 0;
	onGround = false;
}

Player::Player(int sx, int sy)
{
	hImage = LoadGraph("data/image/chara.png");
	x = sx;
	y = sy;
	velocity = 0;
	onGround = false;

}

Player::~Player()
{
}

void Player::Update()
{
}

void Player::Draw()
{
	DrawRectGraph(x , y, 0, 0, 64, 64, hImage, 1);
}
