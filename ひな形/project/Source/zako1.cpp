#include "zako1.h"

zako1::zako1()
{
	hImage = LoadGraph("data/image/zako1.png");
	x = 100;
	y = 100;
	centerX = 100;
	centerY = 100;
	hp = 50;
	isActive = true;

	moveTimer = 0.0f;
	patternTimer = 0.0f;
	patternChangeTime = 300.0f;

	speed = 3.0f;
	direction = 1;
}

zako1::zako1(int sx, int sy)
{
	hImage = LoadGraph("data/image/zako1.png");
	x = sx;
	y = sy;
	centerX = 100;
	centerY = 100;
	hp = 50;
	isActive = true;

	moveTimer = 0.0f;
	patternTimer = 0.0f;
	patternChangeTime = 300.0f;

	speed = 3.0f;
	direction = 1;
}

zako1::~zako1()
{
}

void zako1::Update()
{
}

void zako1::Draw()
{
	DrawGraph(x, y, hImage, TRUE);
}

void zako1::TakeDamage(int damage)
{
}

bool zako1::IsHit(float bx, float by, int rad)
{
	return false;
}

void zako1::UpdateLeftRight()
{
	// ç∂âEà⁄ìÆ
	x += speed * direction;

	// âÊñ í[Ç≈îΩì]
	if (x <= 50 || x >= 640 - 64 - 64 - 64) {
		direction *= -1;
	}
}

