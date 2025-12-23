#include "Boss2.h"
#include"Field.h"
#include"enemyBullet.h"
#include"Player.h"
#include"SceneFactory.h"
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

	// デフォルトの判定サイズ（ボスは大きめ）
	rectWidth = 80.0f;      // 矩形: 80x80
	rectHeight = 80.0f;
	circleRadius = 30.0f;   // 円形: 半径30

	pattern = BossPattern2::CIRCLE;
	moveTimer = 0.0f;
	patternTimer = 0.0f;
	patternChangeTime = 300.0f;

	radius = 150.0f;
	angle = 0.0f;
	angularSpeed = 0.02f;

	figureEightScale = 100.0f;

	speed = 3.0f;
	direction = 1;


	shotTimer = 0.0f;      // 追加
	shotInterval = 0.3f;  // 1秒ごと (60フレーム)
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

	pattern = BossPattern2::CIRCLE;
	moveTimer = 0.0f;
	patternTimer = 0.0f;
	patternChangeTime = 300.0f;

	radius = 100.0f;
	angle = 0.0f;
	angularSpeed = 0.02f;

	figureEightScale = 100.0f;

	speed = 3.0f;
	direction = 1;

	shotTimer = 0.0f;
	shotInterval = 60.0f;
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
