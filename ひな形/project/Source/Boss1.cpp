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

    hp = maxHp;
    isDead = false;

    /// ★追加: 出現直後の無敵設定
    spawnInvincible = true;
    spawnInvincibleTimer = 60.0f; // 60FPS → 1秒
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

    hp = maxHp;
    isDead = false;

    /// ★追加: 出現直後の無敵設定
    spawnInvincible = true;
    spawnInvincibleTimer = 60.0f;
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

    /// ★追加: 出現無敵タイマー処理
    if (spawnInvincible) {
        spawnInvincibleTimer--;
        if (spawnInvincibleTimer <= 0) {
            spawnInvincible = false;
        }
    }

    CheckPhaseTransition();
    shotTimer += 1.0f;

    switch (bulletPhase) {
    case BulletPhase::PHASE_1: UpdatePhase1(); break;
    case BulletPhase::PHASE_2: UpdatePhase2(); break;
    case BulletPhase::PHASE_3: UpdatePhase3(); break;
    }

    if (shotTimer >= shotInterval) {
        switch (bulletPhase) {
        case BulletPhase::PHASE_1: ShotBullet(45.0f, 8.0f); break;
        case BulletPhase::PHASE_2: ShotBullet(30.0f, 3.0f); break;
        case BulletPhase::PHASE_3: ShotBullet(0, 0); break;
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
        shotInterval = 60.0f * 1.7f;
        isCharging = false;
        chargeTimer = 0.0f;
        break;
    }
}

void Boss1::UpdatePhase1()
{
    x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 60.0f;
    y = Field::STAGE_TOP + 20.0f;
}

void Boss1::UpdatePhase2()
{
    x += moveDirection;

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
    x += moveDirection;

    if (x <= Field::STAGE_LEFT + 10.0f) {
        x = Field::STAGE_LEFT + 10.0f;
        moveDirection = 2.0f;
    }
    else if (x >= Field::STAGE_RIGHT - 130.0f) {
        x = Field::STAGE_RIGHT - 130.0f;
        moveDirection = -2.0f;
    }

    if (!isCharging) {
        chargeTimer += 1.0f;
        if (chargeTimer >= 180.0f) {
            isCharging = true;
            chargeTimer = 0.0f;
        }
    }
    else {
        chargeTimer += 1.0f;
        if (chargeTimer >= 60.0f) {
            isCharging = false;
            chargeTimer = 0.0f;
        }
    }
}

void Boss1::ShotBullet(float angle, float num)
{
    Player* player = FindGameObject<Player>();

    if (bulletPhase == BulletPhase::PHASE_1) {
        float angleStep = 360.0f / num;

        for (int i = 0; i < num; i++) {
            float shotAngle = (angleStep * i) * DegToRad;

            float angleDeg = angleStep * i;
            bool isDownward = (angleDeg >= 67.5f && angleDeg <= 112.5f);

            if (isDownward && player != nullptr) {
                float dx = player->GetX() - (x + 60);
                float dy = player->GetY() - (y + 60);
                float length = sqrt(dx * dx + dy * dy);

                if (length > 0) {
                    dx /= length;
                    dy /= length;
                }

                new enemyBullet(x + 60, y + 60, dx * 8.0f, dy * 8.0f, 8.0f, 1);
            }
            else {
                float c1 = cos(shotAngle);
                float s1 = sin(shotAngle);

                new enemyBullet(x + 60, y + 60, c1 * 5.0f, s1 * 5.0f, 8.0f, 0);
            }
        }
    }
    else if (bulletPhase == BulletPhase::PHASE_2) {
        float baseAngle = 90.0f * DegToRad;
        float spreadAngle = angle * DegToRad;

        for (int i = 0; i < num; i++) {
            float offset = (i - (num - 1) / 2.0f) * spreadAngle;
            float shotAngle = baseAngle + offset;

            float c1 = cos(shotAngle);
            float s1 = sin(shotAngle);

            new enemyBullet(x + 32, y + 32, c1 * 5.0f, s1 * 5.0f, 8.0f, 0);
        }
    }
    else if (bulletPhase == BulletPhase::PHASE_3) {
        float baseAngle = 90.0f * DegToRad;
        float spreadRange = 50.0f * DegToRad;
        int spreadCount = 3;

        for (int i = 0; i < spreadCount; i++) {
            float offset = (i - (spreadCount - 1) / 2.0f) * (spreadRange / (spreadCount - 1));
            float shotAngle = baseAngle + offset;

            float c1 = cos(shotAngle);
            float s1 = sin(shotAngle);

            new enemyBullet(x + 32, y + 32, c1 * 4.0f, s1 * 4.0f, 8.0f, 0);
        }

        Player* player = FindGameObject<Player>();
        if (player != nullptr) {
            float rightX = x + 110;
            float rightY = y + 60;

            float dx2 = player->GetX() - rightX;
            float dy2 = player->GetY() - rightY;
            float length2 = sqrt(dx2 * dx2 + dy2 * dy2);

            if (length2 > 0) {
                dx2 /= length2;
                dy2 /= length2;
            }

            new enemyBullet(rightX, rightY, dx2 * 6.0f, dy2 * 6.0f, 8.0f, 1);
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
        DrawFormatString(1000, 70, GetColor(255, 0, 0), "=== BOSS1 ===");
    }
}

void Boss1::TakeDamage(int dmg)
{
    /// ★追加: 登場1秒無敵中は無効
    if (spawnInvincible) return;

    currentHp -= dmg;
    if (currentHp <= 0) {
        currentHp = 0;
        isDead = true;
    }
}

bool Boss1::IsHit(float bx, float by, int rad)
{
    if (!isActive) return false;

    float dx = bx - (x + 100);
    float dy = by - (y + 100);
    float d = sqrt(dx * dx + dy * dy);

    if (d < 100 + rad) {
        PlaySoundFile(GAME_EDEATH_SOUND_PATH, DX_PLAYTYPE_BACK);
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
