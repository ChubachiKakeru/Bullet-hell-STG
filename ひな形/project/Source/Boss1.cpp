#include "Boss1.h"
#include"Field.h"
#include"enemyBullet.h"
#include "Player.h"
#include <cmath>

//#define PI 3.14159265f


Boss1::Boss1() : GameObject()
{
	hImage = LoadGraph("data/image/file/chara/boss1.png");
	x = 300/2;
	y = 200/2;
	centerX = 300;
	centerY = 200;
	hp = 300;
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

	bulletPhase = BulletPhase::PHASE_1;
	phaseTimer = 0.0f;
	phaseChangeTime =15.0f;  // 15秒ごとにフェーズ変更
	shotTimer = 0.0f;
	bulletFireInterval = 0.1f; // 連続発射
	octagonAngle = 0.0f;
	chargeTimer = 0.0f;

	isCharging = false;

	speed = 3.0f;
	direction = 1;


	shotTimer = 0.0f;      // 追加
	shotInterval = 0.3f;  // 1秒ごと (60フレーム)
}

Boss1::Boss1(int sx, int sy)
{
	hImage = LoadGraph("data/image/file/chara/boss1.png");
	x = sx;
	y = sy;
	centerX = sx;
	centerY = sy;
	hp = 300;
	isActive = true;

	pattern = BossPattern::CIRCLE;
	moveTimer = 0.0f;
	patternTimer = 0.0f;
	patternChangeTime = 300.0f;

	radius = 100.0f;
	angle = 0.0f;
	angularSpeed = 0.02f;

	figureEightScale = 100.0f;

	bulletPhase = BulletPhase::PHASE_1;
	phaseTimer = 0.0f;
	phaseChangeTime = 15.0f;  // 15秒ごとにフェーズ変更
	bulletFireTimer = 0.0f;
	bulletFireInterval = 0.1f; // 連続発射
	octagonAngle = 0.0f;
	chargeTimer = 0.0f;

	isCharging = false;

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
	phaseTimer += 1.0f;
	shotTimer += 1.0f;  // ★shotTimerを更新

	// 一定時間でパターン変更
	if (patternTimer >= patternChangeTime) {
		ChangePattern();
		patternTimer = 0.0f;

	}

	// パターンに応じた移動
	/*switch (pattern) {
	case BossPattern::CIRCLE:
		UpdateCircle();
		break;
	case BossPattern::FIGURE_EIGHT:
		UpdateFigureEight();
		break;
	case BossPattern::LEFT_RIGHT:
		UpdateLeftRight();
		break;
	}*/

	// 一定時間でフェーズ変更
	if (phaseTimer >= phaseChangeTime) {
		ChangeBulletPhase();
		phaseTimer = 0.0f;
	}

	

	// 弾発射
	if (shotTimer >= shotInterval) {
		// フェーズに応じた処理
		switch (bulletPhase) {
		case BulletPhase::PHASE_1:
			UpdatePhase1();
			break;
		case BulletPhase::PHASE_2:
			UpdatePhase2();
			UpdateLeftRight();
			break;
		case BulletPhase::PHASE_3:
			UpdatePhase3();
			break;
		}
		shotTimer = 0.0f;
	}
}

//void Boss1::UpdateCircle() {
//	// 円軌道移動
//	angle += angularSpeed;
//	if (angle > 2 *DX_PI) {
//		angle -= 2 *DX_PI;
//	}
//
//	x = centerX + radius * cos(angle);
//	y = centerY + radius * sin(angle);
//}
//
//void Boss1::UpdateFigureEight() {
//	// 八の字移動 (リサージュ曲線)
//	float t = moveTimer * 0.02f;
//	x = centerX + figureEightScale * sin(t);
//	y = centerY + figureEightScale * sin(2 * t) / 2;
//}

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

void Boss1::ChangeBulletPhase() {
	int nextPhase = (static_cast<int>(bulletPhase) + 1) % 3;
	bulletPhase = static_cast<BulletPhase>(nextPhase);

	octagonAngle = 0.0f;
	chargeTimer = 0.0f;
	isCharging = false;
//	shotTimer = 0.0f;  // ★リセット

	// フェーズごとに発射間隔を調整
	if (bulletPhase == BulletPhase::PHASE_1) {
		shotInterval =60.0f;  // 連続発射
	}
	else if (bulletPhase == BulletPhase::PHASE_2) {
		shotInterval = 60.0f;  // 少し遅め
	}
	else if (bulletPhase == BulletPhase::PHASE_3) {
		shotInterval = 60.0f;  // 連続発射
	}

	if (bulletPhase == BulletPhase::PHASE_2 || bulletPhase == BulletPhase::PHASE_3) {
		x = 100.0f;
		moveDirection = 60.0f;
	}
}

void Boss1::ShotBullet(float rad, float num)
{
	Player* player = FindGameObject<Player>();
	float dx = player->GetX() - x;
	float dy = player->GetY() - y;
	float length = sqrt(dx * dx + dy * dy);
	for (int i = 1;i <= num;i++) {
		
		float shotAngle = angle * i * DegToRad;
		float c1 = cos(shotAngle);
		float s1 = sin(shotAngle);
		float newDx1 = dx * c1 - dy * s1;
		float newDy1 = dx * s1 + dy * c1;
		new enemyBullet((float)x + 32, (float)y + 32, newDx1 * 5.0f, newDy1 * 5.0f);

		float c2 = -cos(shotAngle);
		float s2 = -sin(shotAngle);
		float newDx2 = dx * c2 - dy * s2;
		float newDy2 = dx * s2 + dy * c2;
		new enemyBullet((float)x + 32, (float)y + 32, newDx2 *5.0f, newDy2 *5.0f);
	}
	
}

bool Boss1::ShouldFireBullet() {
	// 溜め中は発射しない
	if (isCharging) {
		return false;
	}

	// ★shotTimerで判定
	if (shotTimer >= shotInterval) {
		shotTimer = 0.0f;  // ★リセット
		return true;
	}
	return false;
}

void Boss1::UpdatePhase1() {
	x = 400.0f;
	y = 100.0f;
	octagonAngle += 2.0f;
	shotInterval = 60.0f;  // 連続発射
	ShotBullet(45.0f, 8.0f);
}

void Boss1::UpdatePhase2() {
	x += horizontalSpeed * moveDirection;

	if (x <= 100.0f) {
		x = 100.0f;
		moveDirection = 60.0f;
	}
	else if (x >= 700.0f) {
		x = 700.0f;
		moveDirection = -60.0f;
	}

	shotInterval = 60.0f;
	ShotBullet(270.0f, 5.0f);
}

void Boss1::UpdatePhase3() {
	if (!isCharging) {
		x += horizontalSpeed * moveDirection;

		if (x <= 100.0f) {
			x = 100.0f;
			moveDirection = 60.0f;
		}
		else if (x >= 700.0f) {
			x = 700.0f;
			moveDirection = -60.0f;
		}

		chargeTimer += 60.0f;
		if (chargeTimer >= 3.0f) {
			isCharging = true;
			chargeTimer = 0.0f;
		}
	}
	else {
		chargeTimer += 60.0f;
		if (chargeTimer >= 1.0f) {
			isCharging = false;
			chargeTimer = 0.0f;
		}
	}

	shotInterval = 60.0f;
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
		new enemyBullet((float)x + 32, (float)y + 32, dx * bulletSpeed, dy * bulletSpeed,5.0f);
		
		// 左右にばら撒き (3-way弾)
		float offsetAngle1 = 0.3f;  // 約17度
		float offsetAngle2 = -0.3f;
		float offsetAngle3 = -0.5f;
		float offsetAngle4 = 0.5f;
	

		// 右側の弾
		float cos1 = cos(offsetAngle1);
		float sin1 = sin(offsetAngle1);
		float newDx1 = dx * cos1 - dy * sin1;
		float newDy1 = dx * sin1 + dy * cos1;
		new enemyBullet((int)x + 32, (int)y + 32, newDx1 * bulletSpeed, newDy1 * bulletSpeed);

		float cos4 = cos(offsetAngle4);
		float sin4 = sin(offsetAngle4);
		float newDx4 = dx * cos4 - dy * sin4;
		float newDy4 = dx * sin4 + dy * cos4;
		new enemyBullet((int)x + 32, (int)y + 32, newDx4 * bulletSpeed, newDy4 * bulletSpeed);

		// 左側の弾
		float cos2 = cos(offsetAngle2);
		float sin2 = sin(offsetAngle2);
		float newDx2 = dx * cos2 - dy * sin2;
		float newDy2 = dx * sin2 + dy * cos2;
		new enemyBullet((int)x + 32, (int)y + 32, newDx2 * bulletSpeed, newDy2 * bulletSpeed);

		float cos3 = cos(offsetAngle3);
		float sin3 = sin(offsetAngle3);
		float newDx3 = dx * cos3 - dy * sin3;
		float newDy3 = dx * sin3 + dy * cos3;
		new enemyBullet((int)x + 32, (int)y + 32, newDx3 * bulletSpeed, newDy3 * bulletSpeed);
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

bool Boss1::IsHit(float bx,float by,int rad)
{
	float dx = bx - (x + 60);
	float dy = by - (y + 60);
	float d = sqrt(dx * dx + dy * dy);
	if (d < 60 + rad)
	{
		//DestroyMe();
		//deadCounter = 30;//0.5秒
		TakeDamage(10);
		return true;

	}
	return false;
}

