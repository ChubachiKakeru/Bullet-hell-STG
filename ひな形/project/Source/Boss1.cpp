#include "Boss1.h"
#include "Field.h"
#include "enemyBullet.h"
#include "Player.h"
#include <cmath>

Boss1::Boss1() : Enemy()
{
    bossImage = LoadGraph("data/image/file/chara/boss1.png");
    x = 200.0f;
    y = 200.0f;
    centerX = 300.0f;
    centerY = 200.0f;
    size = 60.0f;

    maxHp = 300;
    currentHp = maxHp;
    Phase2Hp = 200;
    Phase3Hp = 100;

    bulletPhase = BulletPhase::PHASE_1;
    previousPhase = BulletPhase::PHASE_1;

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

Boss1::Boss1(float sx, float sy) : Enemy()
{
    bossImage = LoadGraph("data/image/file/chara/boss1.png");
    x = sx;
    y = sy;
    centerX = sx;
    centerY = sy;
    size = 60.0f;

    maxHp = 300;
    currentHp = maxHp;
    Phase2Hp = 200;
    Phase3Hp = 100;

    bulletPhase = BulletPhase::PHASE_1;
    previousPhase = BulletPhase::PHASE_1;

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

Boss1::~Boss1()
{
    if (bossImage != -1) {
        DeleteGraph(bossImage);
        bossImage = -1;
    }
}

void Boss1::Update()
{
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
        x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 60.0f;
        y = Field::STAGE_TOP + 20.0f;
        shotInterval = 60.0f;
        break;

    case BulletPhase::PHASE_2:
        x = Field::STAGE_LEFT + 50.0f;
        y = Field::STAGE_TOP + 20.0f;
        moveDirection = 2.0f;
        shotInterval = 60.0f;
        break;

    case BulletPhase::PHASE_3:
        x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 60.0f;
        y = Field::STAGE_TOP + 20.0f;
        shotInterval = 60.0f;
        isCharging = false;
        chargeTimer = 0.0f;
        break;
    }
}

void Boss1::UpdatePhase1()
{
    // フェーズ1: 固定位置（ステージ中央上部）
    x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 60.0f;
    y = Field::STAGE_TOP + 20.0f;
}

void Boss1::UpdatePhase2()
{
    // フェーズ2: 左右移動（ステージ範囲内）
    x += moveDirection;

    // ボスの画像サイズ（120px程度）を考慮して範囲制限
    if (x <= Field::STAGE_LEFT + 10.0f) {
        x = Field::STAGE_LEFT + 10.0f;
        moveDirection = 2.0f;
    }
    else if (x >= Field::STAGE_RIGHT - 130.0f) {
        x = Field::STAGE_RIGHT - 130.0f;
        moveDirection = -2.0f;
    }
}

void Boss1::UpdatePhase3()
{
    // フェーズ3: 左右移動（チャージ中も継続、ステージ範囲内）
    x += moveDirection;

    // ボスの画像サイズを考慮して範囲制限
    if (x <= Field::STAGE_LEFT + 10.0f) {
        x = Field::STAGE_LEFT + 10.0f;
        moveDirection = 2.0f;
    }
    else if (x >= Field::STAGE_RIGHT - 130.0f) {
        x = Field::STAGE_RIGHT - 130.0f;
        moveDirection = -2.0f;
    }

    // チャージタイマー管理
    if (!isCharging) {
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
    // フェーズ1の場合は全方位放射状 + 真下だけ自機狙い
    if (bulletPhase == BulletPhase::PHASE_1) {
        float angleStep = 360.0f / num;
        Player* player = FindGameObject<Player>();

        for (int i = 0; i < num; i++) {
            float shotAngle = (angleStep * i) * DegToRad;

            // 真下方向（90度付近）かチェック
            float angleDeg = angleStep * i;
            bool isDownward = (angleDeg >= 67.5f && angleDeg <= 112.5f);

            if (isDownward && player != nullptr) {
                // 真下の弾は自機狙い（画像タイプ1を指定）
                float dx = player->GetX() - x;
                float dy = player->GetY() - y;
                float length = sqrt(dx * dx + dy * dy);

                if (length > 0) {
                    dx /= length;
                    dy /= length;
                }

                // ★ここを修正：第6引数に1を追加（自機狙い用画像）
                new enemyBullet(x + 32, y + 32, dx * 10.0f, dy * 10.0f, 8.0f, 1);
            }
            else {
                // その他の方向は通常の放射状（画像タイプ0を明示）
                float c1 = cos(shotAngle);
                float s1 = sin(shotAngle);

                // ★第6引数に0を追加（通常画像）
                new enemyBullet(x + 32, y + 32, c1 * 5.0f, s1 * 5.0f, 8.0f, 0);
            }
        }
    }
    // フェーズ2とフェーズ3は真下に広がる弾幕（通常画像）
    else {
        float baseAngle = 90.0f * DegToRad;
        float spreadAngle = angle * DegToRad;

        for (int i = 0; i < num; i++) {
            float offset = (i - (num - 1) / 2.0f) * spreadAngle;
            float shotAngle = baseAngle + offset;

            float c1 = cos(shotAngle);
            float s1 = sin(shotAngle);

            // ★通常画像（画像タイプ0）
            new enemyBullet(x + 32, y + 32, c1 * 5.0f, s1 * 5.0f, 8.0f, 0);
        }
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
    if (isActive && bossImage != -1) {
        DrawGraph((int)x, (int)y, bossImage, TRUE);
        DrawFormatString(320, 70, GetColor(255, 0, 0), "Boss HP: %d", currentHp);
    }
}

void Boss1::TakeDamage(int damage)
{
    if (!isActive) return;
    currentHp -= damage;
    hp = currentHp;  // 基底クラスのhpも同期

    if (currentHp <= 0) {
        currentHp = 0;
        hp = 0;
        isActive = false;
        isDead = true;
    }
}

bool Boss1::IsHit(float bx, float by, int rad)
{
    if (!isActive) return false;

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