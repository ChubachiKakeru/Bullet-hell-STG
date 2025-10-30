#include "Boss1.h"
#include"Field.h"
#include"Bullet.h"
#include"DXLib.h"
#include "Player.h"
#include <cmath>

#define PI 3.14159265f


Boss1::Boss1() : GameObject()
{
	hImage = LoadGraph("data/image/bird.png");
	x = 300;
	y = 200;
	centerX = 300;
	centerY = 200;
	hp = 50;
	isActive = true;

	pattern = BossPattern::CIRCLE;
	moveTimer = 0.0f;
	patternTimer = 0.0f;
	patternChangeTime = 300.0f;

	radius = 100.0f;
	angle = 0.0f;
	angularSpeed = 0.02f;

	figureEightScale = 80.0f;

	speed = 3.0f;
	direction = 1;


	shotTimer = 0.0f;      // �ǉ�
	shotInterval = 60.0f;  // 1�b���� (60�t���[��)
}

Boss1::Boss1(int sx, int sy)
{
	hImage = LoadGraph("data/image/boss1.png");
	x = sx;
	y = sy;
	centerX = sx;
	centerY = sy;
	hp = 50;
	isActive = true;

	pattern = BossPattern::CIRCLE;
	moveTimer = 0.0f;
	patternTimer = 0.0f;
	patternChangeTime = 300.0f;

	radius = 100.0f;
	angle = 0.0f;
	angularSpeed = 0.02f;

	figureEightScale = 80.0f;

	speed = 3.0f;
	direction = 1;

	shotTimer = 0.0f;
	shotInterval = 60.0f;
}

	


Boss1::~Boss1()
{
	if (hImage != -1) {
		DeleteGraph(hImage);
	}
}

void Boss1::Update()
{
	if (!isActive) return;

	moveTimer += 1.0f;
	patternTimer += 1.0f;
	shotTimer += 1.0f;

	// ��莞�ԂŃp�^�[���ύX
	if (patternTimer >= patternChangeTime) {
		ChangePattern();
		patternTimer = 0.0f;

	}

	// �p�^�[���ɉ������ړ�
	switch (pattern) {
	case BossPattern::CIRCLE:
		UpdateCircle();
		break;
	case BossPattern::FIGURE_EIGHT:
		UpdateFigureEight();
		break;
	case BossPattern::LEFT_RIGHT:
		UpdateLeftRight();
		break;
	}

	// �e����
	if (shotTimer >= shotInterval) {
		ShootBullet();
		shotTimer = 0.0f;
	}
}

void Boss1::UpdateCircle() {
	// �~�O���ړ�
	angle += angularSpeed;
	if (angle > 2 * PI) {
		angle -= 2 * PI;
	}

	x = centerX + radius * cos(angle);
	y = centerY + radius * sin(angle);
}

void Boss1::UpdateFigureEight() {
	// ���̎��ړ� (���T�[�W���Ȑ�)
	float t = moveTimer * 0.02f;
	x = centerX + figureEightScale * sin(t);
	y = centerY + figureEightScale * sin(2 * t) / 2;
}

void Boss1::UpdateLeftRight() {
	// ���E�ړ�
	x += speed * direction;

	// ��ʒ[�Ŕ��]
	if (x <= 50 || x >= 640 - 64 - 50) {
		direction *= -1;
	}
}

void Boss1::ChangePattern() {
	// �����_���Ƀp�^�[���ύX
	int nextPattern = GetRand(2);
	pattern = (BossPattern)nextPattern;

	// ���S���W�����݈ʒu�ɍX�V
	centerX = x;
	centerY = y;
	moveTimer = 0.0f;
	angle = 0.0f;
}

void Boss1::ShootBullet() {
	// �v���C���[��T��
	Player* player = FindGameObject<Player>();

	if (player != nullptr) {
		// �v���C���[�ւ̕������v�Z
		float dx = player->GetX() - x;
		float dy = player->GetY() - y;
		float length = sqrt(dx * dx + dy * dy);

		if (length > 0) {
			dx /= length;
			dy /= length;
		}

		float bulletSpeed = 5.0f;

		// ����: �v���C���[�_��
		new Bullet((int)x + 32, (int)y + 32, dx * bulletSpeed, dy * bulletSpeed);

		// ���E�ɂ΂�T�� (3-way�e)
		float offsetAngle1 = 0.3f;  // ��17�x
		float offsetAngle2 = -0.3f;

		// �E���̒e
		float cos1 = cos(offsetAngle1);
		float sin1 = sin(offsetAngle1);
		float newDx1 = dx * cos1 - dy * sin1;
		float newDy1 = dx * sin1 + dy * cos1;
		new Bullet((int)x + 32, (int)y + 32, newDx1 * bulletSpeed, newDy1 * bulletSpeed);

		// �����̒e
		float cos2 = cos(offsetAngle2);
		float sin2 = sin(offsetAngle2);
		float newDx2 = dx * cos2 - dy * sin2;
		float newDy2 = dx * sin2 + dy * cos2;
		new Bullet((int)x + 32, (int)y + 32, newDx2 * bulletSpeed, newDy2 * bulletSpeed);
	}
}

void Boss1::Draw()
{
	if (isActive && hImage != -1) {
		DrawGraph((int)x, (int)y, hImage, TRUE);

		// HP�\��
		DrawFormatString(10, 40, GetColor(255, 0, 0), "Boss HP: %d", hp);

		// �f�o�b�O: �p�^�[���\��
		const char* patternName = "";
		switch (pattern) {
		case BossPattern::CIRCLE:
			patternName = "CIRCLE";
			break;
		case BossPattern::FIGURE_EIGHT:
			patternName = "FIGURE_8";
			break;
		case BossPattern::LEFT_RIGHT:
			patternName = "LEFT_RIGHT";
			break;
		}
		DrawFormatString(10, 60, GetColor(255, 255, 0), "Pattern: %s", patternName);
	}
}

void Boss1::TakeDamage(int damage)
{
	hp -= damage;
	if (hp <= 0) {
		isActive = false;
	}
}

//int player, enemy;
//int px = 200;
//int ex = 0, es = 8;
//int WINAPI WinMain2(HINSTANCE, LPSTR, int)
//{
//	ChangeWindowMode(TRUE);
//	SetDrawScreen(DX_SCREEN_BACK);
//	player = LoadGraph("data/image/luffy.png");
//	enemy = LoadGraph("data/image/enel.png");
//
//	ex += es;
//	if (ex <= 0 || ex >= 640 - 64)es = -es;
//	return 0;
//}
