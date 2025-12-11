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

	// デフォルトの判定サイズ
	rectWidth = 30.0f;      // 矩形: 80x80
	rectHeight = 30.0f;
	circleRadius = 30.0f;   // 円形: 半径30

	pattern = Zako1Pattern::CIRCLE;
	moveTimer = 0.0f;
	patternTimer = 0.0f;

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

	// デフォルトの判定サイズ
	rectWidth = 30.0f;      // 矩形: 80x80
	rectHeight = 30.0f;
	circleRadius = 30.0f;   // 円形: 半径30

	pattern = Zako1Pattern::CIRCLE;
	moveTimer = 0.0f;
	patternTimer = 0.0f;

	speed = 3.0f;
	direction = 1;
}

zako1::~zako1()
{
	if (hImage != -1) {
		DeleteGraph(hImage);
	}
}

void zako1::Update()
{
	if (!isActive) return;

	moveTimer += 1.0f;
	patternTimer += 0.4f;
	
	// パターンに応じた移動
	switch (pattern) {
	case Zako1Pattern::LEFT_RIGHT:
		UpdateLeftRight();
		break;
	}
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
	// 左右移動
	x += speed * direction;

	// 画面端で反転
	if (x <= 50 || x >= 640 - 64 - 64 - 64) {
		direction *= -1;
	}
}

