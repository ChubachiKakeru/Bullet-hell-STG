#include "Player.h"
#include"Field.h"

static const float Gravity = 0.2;
static const float V0 = -7.3;
Player::Player()
{
	hImage = LoadGraph("data/image/aoi.png");
	x = 200;
	y = 500;

}


Player::Player(int sx, int sy)
{
	hImage = LoadGraph("data/image/aoi.png");
	x = sx;
	y = sy;


}

Player::~Player()
{

}

void Player::Update()
{
	if (CheckHitKey(KEY_INPUT_D))
	{
		x += 2;
		Field* field = FindGameObject<Field>();
		int push1 = field->HitCheckRight(x + 50, y + 5);
		int push2 = field->HitCheckRight(x + 50, y + 63);
		x -= max(push1, push2);
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		x -= 2;
		Field* field = FindGameObject<Field>();
		int push1 = field->HitCheckLeft(x + 14, y + 5);
		int push2 = field->HitCheckLeft(x + 14, y + 63);
		x -= max(push1, push2);
	}

}

void Player::Draw()
{
	Field* field = FindGameObject<Field>();
	DrawRectGraph(x , y, 0, 0, 64, 64, hImage, 1);
}
