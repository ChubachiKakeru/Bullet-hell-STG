#include "Boss2.h"
#include"Field.h"
#include"enemyBullet.h"
#include"Player.h"
#include <cmath>

Boss2::Boss2()
{
	hImage = LoadGraph("data/image/file/chara/boss2.png");
	x = 300 / 2;
	y = 200 / 2;
	centerX = 300;
	centerY = 200;
	hp = 300;
	isActive = true;

}

Boss2::Boss2(int sx, int sy)
{
	hImage = LoadGraph("data/image/file/chara/boss2.png");
	x = sx;
	y = sy;
	centerX = sx;
	centerY = sy;
	hp = 300;
	isActive = true;

}

Boss2::~Boss2()
{
	if (hImage != -1) {
		DeleteGraph(hImage);
	}
}

void Boss2::Update()
{

}

void Boss2::Draw()
{
	DrawGraph(x, y, hImage, TRUE);
}

void Boss2::TakeDamage(int damage)
{
}

bool Boss2::IsHit(float bx, float by, int rad)
{
	return false;
}

void Boss2::UpdateCircle()
{
}

void Boss2::UpdateFigureEight()
{
}

void Boss2::UpdateLeftRight()
{
}

void Boss2::ChangePattern()
{
}

void Boss2::ShootBullet()
{
}
