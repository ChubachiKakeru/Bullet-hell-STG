#include "Boss2.h"
#include"Field.h"
#include"enemyBullet.h"
#include"Player.h"
#include <cmath>

Boss2::Boss2()
{
	bossImage = LoadGraph("data/image/file/chara/boss2.png");
	x = 200.0f;
	y = 200.0f;
	centerX = 300.0f;
	centerY = 200.0f;
	size = 60.0f;

	maxHp = 300;
	currentHp = maxHp;
	Phase2Hp = 200;
	Phase3Hp = 100;

	bulletPhase = BulletPhase2::PHASE_1;
	previousPhase = BulletPhase2::PHASE_1;

	shotTimer = 0.0f;
	shotInterval = 120.0f;

	moveDirection = 1.0f;

	isCharging = false;
	chargeTimer = 0.0f;

	isActive = true;

	// 基底クラスの変数も初期化
	hp = maxHp;
	isDead = false;
}

Boss2::Boss2(float sx, float sy)
{
	bossImage = LoadGraph("data/image/file/chara/boss2.png");
	x = sx;
	y = sy;
	centerX = sx;
	centerY = sy;
	size = 60.0f;

	maxHp = 300;
	currentHp = maxHp;
	Phase2Hp = 200;
	Phase3Hp = 100;

	bulletPhase = BulletPhase2::PHASE_1;
	previousPhase = BulletPhase2::PHASE_1;

	shotTimer = 0.0f;
	shotInterval = 120.0f;

	moveDirection = 1.0f;

	isCharging = false;
	chargeTimer = 0.0f;

	isActive = true;

	// 基底クラスの変数も初期化
	hp = maxHp;
	isDead = false;
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

void Boss2::CheckPhaseTransition()
{

}

void Boss2::OnPhaseChanged(int newPhase)
{

}

void Boss2::UpdatePhase1()
{

}

void Boss2::UpdatePhase2()
{

}

void Boss2::UpdatePhase3()
{

}

void Boss2::ShotBullet(float angle, float num)
{

}

void Boss2::ShootBullet()
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
