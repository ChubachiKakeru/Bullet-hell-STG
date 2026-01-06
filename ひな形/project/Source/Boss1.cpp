#include "Boss1.h"
#include "Field.h"
#include "enemyBullet.h"
#include "Player.h"
#include <cmath>

Boss1::Boss1() : GameObject()
{
    hImage = LoadGraph("data/image/file/chara/boss1.png");
    x = 300.0f;
    y = 200.0f;
    centerX = 300.0f;
    centerY = 200.0f;

    maxHp = 300;
    currentHp = maxHp;
    Phase2Hp = 200;
    Phase3Hp = 100;

    bulletPhase = BulletPhase::PHASE_1;
    previousPhase = BulletPhase::PHASE_1;

    shotTimer = 0.0f;
    shotInterval = 60.0f;

    moveDirection = 1.0f;

    isCharging = false;
    chargeTimer = 0.0f;

    isActive = true;
}

Boss1::Boss1(int sx, int sy)
{
    hImage = LoadGraph("data/image/file/chara/boss1.png");
    x = (float)sx;
    y = (float)sy;
    centerX = (float)sx;
    centerY = (float)sy;

    maxHp = 300;
    currentHp = maxHp;
    Phase2Hp = 200;
    Phase3Hp = 100;

    bulletPhase = BulletPhase::PHASE_1;
    previousPhase = BulletPhase::PHASE_1;

    shotTimer = 0.0f;
    shotInterval = 60.0f;

    moveDirection = 1.0f;

    isCharging = false;
    chargeTimer = 0.0f;

    isActive = true;
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

    // HPベースのフェーズチェック
    CheckPhaseTransition();

    // 弾発射タイマー更新
    shotTimer += 1.0f;

    // 各フェーズの更新
    switch (bulletPhase) {
    case BulletPhase::PHASE_1:
        UpdatePhase1();
        break;
    case BulletPhase::PHASE_2:
        UpdatePhase2();
        break;
    case BulletPhase::PHASE_3:
        UpdatePhase3();
        break;
    }

    // 弾発射判定
    if (shotTimer >= shotInterval) {
        switch (bulletPhase) {
        case BulletPhase::PHASE_1:
            ShotBullet(45.0f, 8.0f);
            break;
        case BulletPhase::PHASE_2:
            ShotBullet(30.0f, 3.0f);
            break;
        case BulletPhase::PHASE_3:
            if (!isCharging) {
                ShotBullet(25.0f, 5.0f);
            }
            break;
        }
        shotTimer = 0.0f;
    }
}

void Boss1::CheckPhaseTransition()
{
    previousPhase = bulletPhase;

    if (currentHp <= Phase3Hp) {
        bulletPhase = BulletPhase::PHASE_3;
    }
    else if (currentHp <= Phase2Hp) {
        bulletPhase = BulletPhase::PHASE_2;
    }
    else {
        bulletPhase = BulletPhase::PHASE_1;
    }

    if (bulletPhase != previousPhase) {
        OnPhaseChanged(GetCurrentPhaseNumber());
    }
}

void Boss1::OnPhaseChanged(int newPhase)
{
    printfDx("フェーズ %d に移行！HP: %d\n", newPhase, currentHp);

    // フェーズごとの初期化
    switch (bulletPhase) {
    case BulletPhase::PHASE_1:
        x = 400.0f;
        y = 100.0f;
        shotInterval = 60.0f;
        break;

    case BulletPhase::PHASE_2:
        x = 100.0f;
        y = 100.0f;
        moveDirection = 2.0f;
        shotInterval = 60.0f;
        break;

    case BulletPhase::PHASE_3:
        x = 400.0f;
        y = 100.0f;
        shotInterval = 60.0f;
        isCharging = false;
        chargeTimer = 0.0f;
        break;
    }
}

void Boss1::UpdatePhase1()
{
    // フェーズ1: 固定位置
    x = 400.0f;
    y = 100.0f;
}

void Boss1::UpdatePhase2()
{
    // フェーズ2: 左右移動
    x += moveDirection;

    if (x <= 100.0f) {
        x = 100.0f;
        moveDirection = 2.0f;
    }
    else if (x >= 700.0f) {
        x = 700.0f;
        moveDirection = -2.0f;
    }
}

void Boss1::UpdatePhase3()
{
    // フェーズ3: 左右移動 + チャージ
    if (!isCharging) {
        x += moveDirection;

        if (x <= 100.0f) {
            x = 100.0f;
            moveDirection = 2.0f;
        }
        else if (x >= 700.0f) {
            x = 700.0f;
            moveDirection = -2.0f;
        }

        chargeTimer += 1.0f;
        if (chargeTimer >= 180.0f) {  // 3秒
            isCharging = true;
            chargeTimer = 0.0f;
        }
    }
    else {
        chargeTimer += 1.0f;
        if (chargeTimer >= 60.0f) {  // 1秒
            isCharging = false;
            chargeTimer = 0.0f;
        }
    }
}

void Boss1::ShotBullet(float angle, float num)
{
    for (int i = 0; i < num; i++) {
        float shotAngle = angle * i * DegToRad;
        float c1 = cos(shotAngle);
        float s1 = sin(shotAngle);

        new enemyBullet(x + 32, y + 32, c1 * 5.0f, s1 * 5.0f);
    }
}

void Boss1::ShootBullet()
{
    Player* player = FindGameObject<Player>();
    if (player == nullptr) return;

    float dx = player->GetX() - x;
    float dy = player->GetY() - y;
    float length = sqrt(dx * dx + dy * dy);

    if (length > 0) {
        dx /= length;
        dy /= length;
    }

    float bulletSpeed = 5.0f;

    // 5-way弾
    float angles[] = { 0.0f, 0.3f, -0.3f, 0.5f, -0.5f };

    for (int i = 0; i < 5; i++) {
        float angle = angles[i];
        float cosA = cos(angle);
        float sinA = sin(angle);
        float newDx = dx * cosA - dy * sinA;
        float newDy = dx * sinA + dy * cosA;

        new enemyBullet(x + 32, y + 32, newDx * bulletSpeed, newDy * bulletSpeed);
    }
}

void Boss1::Draw()
{
    if (isActive && hImage != -1) {
        DrawGraph((int)x, (int)y, hImage, TRUE);
        DrawFormatString(10, 40, GetColor(255, 0, 0), "Boss HP: %d", currentHp);
    }
}

void Boss1::TakeDamage(int damage)
{
    if (!isActive) return;
    currentHp -= damage;
    if (currentHp <= 0) {
        currentHp = 0;
        isActive = false;
    }
}

bool Boss1::IsHit(float bx, float by, int rad)
{
    float dx = bx - (x + 60);
    float dy = by - (y + 60);
    float d = sqrt(dx * dx + dy * dy);

    if (d < 60 + rad) {
        TakeDamage(100);
        return true;
    }
    return false;
}

float Boss1::GetHpPercent() const
{
    if (maxHp == 0) return 0.0f;
    return (float)currentHp / (float)maxHp;
}

int Boss1::GetCurrentPhaseNumber() const
{
    return (int)bulletPhase + 1;
}