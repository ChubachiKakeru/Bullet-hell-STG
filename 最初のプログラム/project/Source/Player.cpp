#include "Player.h"

Player::Player()
{
	image = LoadGraph("data/chara.png");
	posX = 100;
	posY = 100;
}

Player::~Player()
{
}

void Player::Update()
{
	posX += 1;
}

void Player::Draw()
{
	DrawRectGraph(posX, posY, 0/*CHR_SIZE * pat*/, 0/*CHR_SIZE * dir*/,
		64/*CHR_SIZE*/, 64/*CHR_SIZE*/, image, 1);
}
