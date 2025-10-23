#include "Shot.h"



Shot::Shot() : active(false)
{
	x = 0;
	y = 0;
	sf = false;
	playerX = 0;
	playerY = 0;
	speed = 5;
	hImage = LoadGraph("data/image/stone.png");

}

Shot::~Shot()
{
}

void Shot::Update()
{
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		if (!sf && !active)
		{
			Fire(playerX + 24, playerY);
		}
		sf = true;
	}
	else {
		sf = false;
	}

	if (!active)return;
	y -= speed;
	if (y < 0)
	{
		active = false;
	}

}

void Shot::Draw()
{
	if (active && hImage != -1)
	{
		DrawGraph(sx, sy, hImage, 1);
	}
}

void Shot::Fire(int sx, int sy)
{
	if (!active)
	{
		x = sx;
		y = sy;
		active = true;
	}
}
