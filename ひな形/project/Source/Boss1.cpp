#include "Boss1.h"
#include"Field.h"
#include"Bullet.h"
#include"DXLib.h"
#include "Player.h"
#include <cmath>

#define PI 3.14159265f


Boss1::Boss1() : GameObject()
{
	hImage = LoadGraph("data/image/file/chara/boss1.png");
	x = 300/2;
	y = 200/2;
	centerX = 300;
	centerY = 200;
	hp = 50;
	isActive = true;

	// デフォルトの判定サイズ（ボスは大きめ）
	rectWidth = 80.0f;      // 矩形: 80x80
	rectHeight = 80.0f;
	circleRadius = 30.0f;   // 円形: 半径30

	pattern = BossPattern::CIRCLE;
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
	shotInterval = 60.0f;  // 1秒ごと (60フレーム)
}

Boss1::Boss1(int sx, int sy)
{
	hImage = LoadGraph("data/image/file/chara/boss1.png");
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

	figureEightScale = 100.0f;

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
	patternTimer += 0.4f;
	shotTimer += 1.0f;

	// 一定時間でパターン変更
	if (patternTimer >= patternChangeTime) {
		ChangePattern();
		patternTimer = 0.0f;

	}

	// パターンに応じた移動
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

	// 弾発射
	if (shotTimer >= shotInterval) {
		ShootBullet();
		shotTimer = 0.0f;
	}
}

void Boss1::UpdateCircle() {
	// 円軌道移動
	angle += angularSpeed;
	if (angle > 2 * PI) {
		angle -= 2 * PI;
	}

	x = centerX + radius * cos(angle);
	y = centerY + radius * sin(angle);
}

void Boss1::UpdateFigureEight() {
	// 八の字移動 (リサージュ曲線)
	float t = moveTimer * 0.02f;
	x = centerX + figureEightScale * sin(t);
	y = centerY + figureEightScale * sin(2 * t) / 2;
}

void Boss1::UpdateLeftRight() {
	// 左右移動
	x += speed * direction;

	// 画面端で反転
	if (x <= 50 || x >= 640 - 64 - 64 - 64) {
		direction *= -1;
	}
}

void Boss1::ChangePattern() {
	// パターン変更
	switch (pattern) {
	case BossPattern::CIRCLE:
		pattern = BossPattern::FIGURE_EIGHT;
		break;
	case BossPattern::FIGURE_EIGHT:
		pattern = BossPattern::LEFT_RIGHT;
		break;
	case BossPattern::LEFT_RIGHT:
		pattern = BossPattern::CIRCLE;
		break;
	}

	// 中心座標を初期位置に更新
	centerX = 200;
	centerY = 100;
	moveTimer = 0.0f;
	angle = 0.0f;
}

void Boss1::ShootBullet() {
	// プレイヤーを探す
	Player* player = FindGameObject<Player>();

	if (player != nullptr) {
		// プレイヤーへの方向を計算
		float dx = player->GetX() - x;
		float dy = player->GetY() - y;
		float length = sqrt(dx * dx + dy * dy);

		if (length > 0) {
			dx /= length;
			dy /= length;
		}

		float bulletSpeed = 5.0f;

		// 中央: プレイヤー狙い
		new Bullet((int)x + 32, (int)y + 32, dx * bulletSpeed, dy * bulletSpeed);

		// 左右にばら撒き (3-way弾)
		float offsetAngle1 = 0.3f;  // 約17度
		float offsetAngle2 = -0.3f;

		// 右側の弾
		float cos1 = cos(offsetAngle1);
		float sin1 = sin(offsetAngle1);
		float newDx1 = dx * cos1 - dy * sin1;
		float newDy1 = dx * sin1 + dy * cos1;
		new Bullet((int)x + 32, (int)y + 32, newDx1 * bulletSpeed, newDy1 * bulletSpeed);

		// 左側の弾
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

		// HP表示
		DrawFormatString(10, 40, GetColor(255, 0, 0), "Boss HP: %d", hp);

		// デバッグ: パターン表示
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

