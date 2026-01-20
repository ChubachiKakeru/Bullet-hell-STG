#include "Boss2.h"
#include"Field.h"
#include"enemyBullet.h"
#include"Player.h"
#include <cmath>

Boss2::Boss2()
{
	bossImage = LoadGraph("data/image/file/chara/boss2.png");
	enemyDeathSoundHandle = LoadSoundMem(GAME_EDEATH_SOUND_PATH);
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

	/// ★追加: 出現直後の無敵設定
	spawnInvincible = true;
	spawnInvincibleTimer = 60.0f;
}

Boss2::Boss2(float sx, float sy)
{
	bossImage = LoadGraph("data/image/file/chara/boss2.png");
	enemyDeathSoundHandle = LoadSoundMem(GAME_EDEATH_SOUND_PATH);
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

	/// ★追加: 出現直後の無敵設定
	spawnInvincible = true;
	spawnInvincibleTimer = 60.0f; // 60FPS → 1秒
}

Boss2::~Boss2()
{
	if (bossImage != -1) {
		DeleteGraph(bossImage);
		bossImage = -1;
	}
	DeleteSoundMem(enemyDeathSoundHandle);
}

void Boss2::Update()
{
	if (spawnInvincible)
	{
		spawnInvincibleTimer -= 1.0f;
		if (spawnInvincibleTimer <= 0.0f)
		{
			spawnInvincible = false;
		}
	}

	if (!isActive) {
		isDead = true;
		return;
	}

	// HPベースのフェーズチェック
	CheckPhaseTransition();

	// 弾発射タイマー更新
	shotTimer += 1.0f;

	// 各フェーズの更新
	switch (bulletPhase) {
	case BulletPhase2::PHASE_1:
		UpdatePhase1();
		break;
	case BulletPhase2::PHASE_2:
		UpdatePhase2();
		break;
	case BulletPhase2::PHASE_3:
		UpdatePhase3();
		break;
	}

	// 弾発射判定
	if (shotTimer >= shotInterval) {
		switch (bulletPhase) {
		case BulletPhase2::PHASE_1:
			ShotBullet(45.0f, 8.0f);
			break;
		case BulletPhase2::PHASE_2:
			ShotBullet(30.0f, 3.0f);
			break;
		case BulletPhase2::PHASE_3:
			if (!isCharging) {
				ShotBullet(25.0f, 5.0f);
			}
			break;
		}
		shotTimer = 0.0f;
	}
}

void Boss2::CheckPhaseTransition()
{
	previousPhase = bulletPhase;

	if (currentHp <= Phase3Hp) {
		bulletPhase = BulletPhase2::PHASE_3;
	}
	else if (currentHp <= Phase2Hp) {
		bulletPhase = BulletPhase2::PHASE_2;
	}
	else {
		bulletPhase = BulletPhase2::PHASE_1;
	}

	if (bulletPhase != previousPhase) {
		OnPhaseChanged(GetCurrentPhaseNumber());
	}
}

void Boss2::OnPhaseChanged(int newPhase)
{
	printfDx("フェーズ %d に移行！HP: %d\n", newPhase, currentHp);

	// フェーズごとの初期化
	switch (bulletPhase) {
	case BulletPhase2::PHASE_1:
		x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 60.0f;
		y = Field::STAGE_TOP + 20.0f;
		shotInterval = 60.0f;
		break;

	case BulletPhase2::PHASE_2:
		x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 60.0f;
		y = Field::STAGE_TOP + 20.0f;
		shotInterval = 60.0f;
		break;

	case BulletPhase2::PHASE_3:
		x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 60.0f;
		y = Field::STAGE_TOP + 20.0f;
		shotInterval = 60.0f;
		isCharging = false;
		chargeTimer = 0.0f;
		break;
	}
}

void Boss2::UpdatePhase1()
{
	// フェーズ1：動かない
}

void Boss2::UpdatePhase2()
{
	// フェーズ2：動かない
}

void Boss2::UpdatePhase3()
{
	// フェーズ3：回転角だけ更新（ボスは動かない）
	moveDirection += 0.05f;  // 回転スピード（調整可）
}

void Boss2::ShotBullet(float angle, float num)
{
	// ==========================
	// フェーズ1：自機狙い扇3段
	// ==========================
	if (bulletPhase == BulletPhase2::PHASE_1)
	{
		Player* player = FindGameObject<Player>();
		if (!player) return;

		float dx = player->GetX() - x;
		float dy = player->GetY() - y;
		float baseAngle = atan2f(dy, dx);

		const int bulletCount = 9;
		const float spread = 0.7f;

		float speeds[3] = { 4.0f, 5.5f, 7.0f };

		for (int layer = 0; layer < 3; layer++)
		{
			for (int i = 0; i < bulletCount; i++)
			{
				float t = (float)i / (bulletCount - 1);
				float a = baseAngle - spread * 0.5f + spread * t;

				float vx = cosf(a) * speeds[layer];
				float vy = sinf(a) * speeds[layer];

				new enemyBullet(x + 32, y + 32, vx, vy, 8.0f, 1); // 炎
			}
		}
	}
	// ==========================
	// フェーズ2：炎（自機狙い）＋白（下）
	// ==========================
	else if (bulletPhase == BulletPhase2::PHASE_2)
	{
		Player* player = FindGameObject<Player>();
		if (!player) return;

		float dx = player->GetX() - x;
		float dy = player->GetY() - y;
		float baseAngle = atan2f(dy, dx);

		// --- 炎 ---
		const int fireCount = 5;
		const float fireSpread = 0.4f;

		for (int i = 0; i < fireCount; i++)
		{
			float t = (float)i / (fireCount - 1);
			float a = baseAngle - fireSpread * 0.5f + fireSpread * t;

			new enemyBullet(x + 32, y + 32,
				cosf(a) * 5.0f, sinf(a) * 5.0f, 8.0f, 1);
		}

		// --- 白 ---
		const int whiteCount = 7;

		for (int i = 0; i < whiteCount; i++)
		{
			float offset = (i - (whiteCount - 1) * 0.5f) * 0.2f;
			float a = PI / 2.0f + offset;

			new enemyBullet(x + 32, y + 32,
				cosf(a) * 3.0f, sinf(a) * 3.0f, 8.0f, 0);
		}
	}
	// ==========================
	// フェーズ3：6列回転弾幕
	// ==========================
	else if (bulletPhase == BulletPhase2::PHASE_3)
	{
		const int lineCount = 6;
		const float speed = 4.0f;

		for (int i = 0; i < lineCount; i++)
		{
			float a = angle + (2.0f * PI / lineCount) * i;

			new enemyBullet(x + 32, y + 32,
				cosf(a) * speed, sinf(a) * speed, 8.0f, 1);
		}
	}
}

void Boss2::ShootBullet()
{
	Player* player = FindGameObject<Player>();
	if (!player) return;

	float dx = player->GetX() - x;
	float dy = player->GetY() - y;
	float length = sqrt(dx * dx + dy * dy);

	if (length > 0) {
		dx /= length;
		dy /= length;
	}

	float bulletSpeed = 5.0f;

	float angles[] = { 0.0f, 0.3f, -0.3f, 0.5f, -0.5f };

	for (int i = 0; i < 5; i++) {
		float a = angles[i];
		float cosA = cos(a);
		float sinA = sin(a);

		float newDx = dx * cosA - dy * sinA;
		float newDy = dx * sinA + dy * cosA;

		new enemyBullet(x + 32, y + 32, newDx * bulletSpeed, newDy * bulletSpeed, 8.0f, 1);
	}
}

void Boss2::Draw()
{
	if (isActive && bossImage != -1) {

		/// ★無敵中は点滅描画でフィードバック
		if (spawnInvincible) {
			if (((int)(spawnInvincibleTimer / 5)) % 2 == 0)
			{
				DrawGraph((int)x, (int)y, bossImage, TRUE);
			}
		}
		else {
			DrawGraph((int)x, (int)y, bossImage, TRUE);
		}

		DrawFormatString(1000, 100, GetColor(255, 0, 0), "BOSS HP: %d", currentHp);
		DrawFormatString(1000, 70, GetColor(255, 0, 0), "=== BOSS2 ===");
	}
}

void Boss2::TakeDamage(int dmg)
{
	/// ★追加: 登場1秒無敵中は無効
	if (spawnInvincible) return;

	currentHp -= dmg;
	if (currentHp <= 0) {
		currentHp = 0;
		isDead = true;
	}
}

bool Boss2::IsHit(float bx, float by, int rad)
{
	if (!isActive) return false;

	float dx = bx - (x + 60);
	float dy = by - (y + 60);
	float d = sqrt(dx * dx + dy * dy);

	if (d < 60 + rad) {
		PlaySoundMem(enemyDeathSoundHandle, DX_PLAYTYPE_BACK);
		TakeDamage(100);
		return true;
	}
	return false;
}

float Boss2::GetHpPercent() const
{
	if (maxHp == 0) return 0.0f;
	return (float)currentHp / (float)maxHp;
}

int Boss2::GetCurrentPhaseNumber() const
{
	return (int)bulletPhase + 1;
}