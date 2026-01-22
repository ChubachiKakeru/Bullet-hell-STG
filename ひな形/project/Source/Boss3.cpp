#include "Boss3.h"
#include "Field.h"
#include "EnemyBullet2.h"
#include "Player.h"
#include "../Library/GameObject.h"
#include <cmath>

Boss3::Boss3() : Enemy()
{
    bossImage = LoadGraph("data/image/file/chara/boss3.png");
    x = 200.0f;
    y = 200.0f;
    centerX = 300.0f;
    centerY = 200.0f;
    size = 60.0f;

    maxHp = 300;
    currentHp = maxHp;
    Phase2Hp = 225;
    Phase3Hp = 150;
    Phase4Hp = 75;

    bulletPhase = BulletPhase3::PHASE_1;
    previousPhase = BulletPhase3::PHASE_1;

    shotTimer = 0.0f;
    shotInterval = 60.0f;

    moveDirection = 1.0f;

    isCharging = false;
    chargeTimer = 0.0f;

    isActive = true;

    hp = maxHp;
    isDead = false;

    spawnInvincible = true;
    spawnInvincibleTimer = 60.0f;

    scatterShotTimer = 0.0f;
    scatterShotInterval = 45.0f;
    aimedShotTimer = 0.0f;
    aimedShotInterval = 90.0f;

    activeBeamCount = 0;
    beamSpawnTimer = 0.0f;
    beamSpawnInterval = 120.0f;
    InitBeams();

    spiralAngle = 0.0f;
    spiralSpeed = 0.08f;
    spiralReversed = false;
    spiralTimer = 0.0f;

    diagonalBeamTimer = 0.0f;
    diagonalBeamInterval = 180.0f;
    for (int i = 0; i < 2; i++) {
        diagonalBeams[i].active = false;
        diagonalBeams[i].timer = 0.0f;
    }
}

Boss3::Boss3(float sx, float sy) : Enemy()
{
    bossImage = LoadGraph("data/image/file/chara/boss3.png");
    x = sx;
    y = sy;
    centerX = sx;
    centerY = sy;
    size = 60.0f;

    maxHp = 300;
    currentHp = maxHp;
    Phase2Hp = 225;
    Phase3Hp = 150;
    Phase4Hp = 75;

    bulletPhase = BulletPhase3::PHASE_1;
    previousPhase = BulletPhase3::PHASE_1;

    shotTimer = 0.0f;
    shotInterval = 60.0f;

    moveDirection = 1.0f;

    isCharging = false;
    chargeTimer = 0.0f;

    isActive = true;

    hp = maxHp;
    isDead = false;

    spawnInvincible = true;
    spawnInvincibleTimer = 60.0f;

    scatterShotTimer = 0.0f;
    scatterShotInterval = 45.0f;
    aimedShotTimer = 0.0f;
    aimedShotInterval = 90.0f;

    activeBeamCount = 0;
    beamSpawnTimer = 0.0f;
    beamSpawnInterval = 120.0f;
    InitBeams();

    spiralAngle = 0.0f;
    spiralSpeed = 0.08f;
    spiralReversed = false;
    spiralTimer = 0.0f;

    diagonalBeamTimer = 0.0f;
    diagonalBeamInterval = 180.0f;
    for (int i = 0; i < 2; i++) {
        diagonalBeams[i].active = false;
        diagonalBeams[i].timer = 0.0f;
    }
}

Boss3::~Boss3()
{
    if (bossImage != -1) {
        DeleteGraph(bossImage);
        bossImage = -1;
    }
}

void Boss3::Update()
{
    if (!isActive) {
        isDead = true;
        return;
    }

    if (spawnInvincible)
    {
        spawnInvincibleTimer -= 1.0f;
        if (spawnInvincibleTimer <= 0.0f)
        {
            spawnInvincible = false;
        }
    }

    CheckPhaseTransition();

    switch (bulletPhase) {
    case BulletPhase3::PHASE_1:
        UpdatePhase1();
        break;
    case BulletPhase3::PHASE_2:
        UpdatePhase2();
        break;
    case BulletPhase3::PHASE_3:
        UpdatePhase3();
        break;
    case BulletPhase3::PHASE_4:
        UpdatePhase4();
        break;
    }
}

void Boss3::CheckPhaseTransition()
{
    previousPhase = bulletPhase;

    if (currentHp <= Phase4Hp) {
        bulletPhase = BulletPhase3::PHASE_4;
    }
    else if (currentHp <= Phase3Hp) {
        bulletPhase = BulletPhase3::PHASE_3;
    }
    else if (currentHp <= Phase2Hp) {
        bulletPhase = BulletPhase3::PHASE_2;
    }
    else {
        bulletPhase = BulletPhase3::PHASE_1;
    }

    if (bulletPhase != previousPhase) {
        OnPhaseChanged(GetCurrentPhaseNumber());
    }
}

void Boss3::OnPhaseChanged(int newPhase)
{
    printfDx("フェーズ %d に移行！HP: %d\n", newPhase, currentHp);

    switch (bulletPhase) {
    case BulletPhase3::PHASE_1:
        x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 60.0f;
        y = Field::STAGE_TOP + 20.0f;
        scatterShotTimer = 0.0f;
        aimedShotTimer = 0.0f;
        break;

    case BulletPhase3::PHASE_2:
        x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 60.0f;
        y = Field::STAGE_TOP + 20.0f;
        InitBeams();
        beamSpawnTimer = 0.0f;
        break;

    case BulletPhase3::PHASE_3:
        x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 60.0f;
        y = Field::STAGE_TOP + 20.0f;
        spiralAngle = 0.0f;
        spiralSpeed = 0.08f;
        spiralReversed = false;
        spiralTimer = 0.0f;
        shotTimer = 0.0f;
        shotInterval = 8.0f;
        break;

    case BulletPhase3::PHASE_4:
        x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 60.0f;
        y = Field::STAGE_TOP + 20.0f;
        scatterShotTimer = 0.0f;
        aimedShotTimer = 0.0f;
        diagonalBeamTimer = 0.0f;
        for (int i = 0; i < 2; i++) {
            diagonalBeams[i].active = false;
            diagonalBeams[i].timer = 0.0f;
        }
        break;
    }
}

void Boss3::InitBeams()
{
    for (int i = 0; i < 4; i++) {
        beams[i].isActive = false;
        beams[i].isWarning = false;
        beams[i].warningTimer = 0.0f;
        beams[i].activeTimer = 0.0f;
        beams[i].angle = 0.0f;
    }
    activeBeamCount = 0;
}

void Boss3::UpdatePhase1()
{
    x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 60.0f;
    y = Field::STAGE_TOP + 20.0f;

    scatterShotTimer += 1.0f;
    aimedShotTimer += 1.0f;

    if (scatterShotTimer >= scatterShotInterval) {
        ShootScatterBullets();
        scatterShotTimer = 0.0f;
    }

    if (aimedShotTimer >= aimedShotInterval) {
        ShootAimedBullets();
        aimedShotTimer = 0.0f;
    }
}

void Boss3::UpdatePhase2()
{
    x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 60.0f;
    y = Field::STAGE_TOP + 20.0f;

    beamSpawnTimer += 1.0f;
    if (beamSpawnTimer >= beamSpawnInterval && activeBeamCount < 4) {
        SpawnBeam();
        beamSpawnTimer = 0.0f;
    }

    UpdateBeams();
}

void Boss3::UpdatePhase3()
{
    x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 60.0f;
    y = Field::STAGE_TOP + 20.0f;

    spiralTimer += 1.0f;

    if (spiralTimer >= 300.0f && !spiralReversed) {
        spiralSpeed = -spiralSpeed;
        spiralReversed = true;
    }

    spiralAngle += spiralSpeed;

    shotTimer += 1.0f;
    if (shotTimer >= shotInterval) {
        ShootSpiralBullets();
        shotTimer = 0.0f;
    }
}

void Boss3::UpdatePhase4()
{
    x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 60.0f;
    y = Field::STAGE_TOP + 20.0f;

    scatterShotTimer += 1.0f;
    aimedShotTimer += 1.0f;
    diagonalBeamTimer += 1.0f;

    if (scatterShotTimer >= 40.0f) {
        ShootScatterBullets();
        scatterShotTimer = 0.0f;
    }

    if (aimedShotTimer >= 80.0f) {
        ShootAimedBullets();
        aimedShotTimer = 0.0f;
    }

    if (diagonalBeamTimer >= diagonalBeamInterval) {
        SpawnDiagonalBeam();
        diagonalBeamTimer = 0.0f;
    }

    UpdateDiagonalBeams();
}

void Boss3::ShootScatterBullets()
{
    const int bulletCount = 24;
    float angleStep = (3.14159265f * 2.0f) / bulletCount;

    for (int i = 0; i < bulletCount; i++) {
        float angle = angleStep * i;
        float vx = cosf(angle) * 3.0f;
        float vy = sinf(angle) * 3.0f;

        new EnemyBullet2(x + 32, y + 32, vx, vy, 6.0f, 0);
    }
}

void Boss3::ShootAimedBullets()
{
    Player* player = FindGameObject<Player>();
    if (!player) return;

    float dx = player->GetX() - x;
    float dy = player->GetY() - y;
    float baseAngle = atan2f(dy, dx);

    const int bulletCount = 5;
    const float spread = 0.6f;

    for (int i = 0; i < bulletCount; i++) {
        float t = (float)i / (bulletCount - 1);
        float angle = baseAngle - spread * 0.5f + spread * t;

        float speed = 5.0f;
        float vx = cosf(angle) * speed;
        float vy = sinf(angle) * speed;

        new EnemyBullet2(x + 32, y + 32, vx, vy, 12.0f, 1);
    }
}

void Boss3::SpawnBeam()
{
    Player* player = FindGameObject<Player>();
    if (!player) return;

    for (int i = 0; i < 4; i++) {
        if (!beams[i].isActive && !beams[i].isWarning) {
            beams[i].isWarning = true;
            beams[i].warningTimer = 60.0f;

            float dx = player->GetX() - (x + 32);
            float dy = player->GetY() - (y + 32);
            beams[i].angle = atan2f(dy, dx);

            activeBeamCount++;
            break;
        }
    }
}

void Boss3::UpdateBeams()
{
    for (int i = 0; i < 4; i++) {
        if (beams[i].isWarning) {
            beams[i].warningTimer -= 1.0f;
            if (beams[i].warningTimer <= 0.0f) {
                beams[i].isWarning = false;
                beams[i].isActive = true;
                beams[i].activeTimer = 90.0f;
            }
        }
        else if (beams[i].isActive) {
            beams[i].activeTimer -= 1.0f;
            if (beams[i].activeTimer <= 0.0f) {
                beams[i].isActive = false;
                activeBeamCount--;
            }
        }
    }
}

void Boss3::ShootSpiralBullets()
{
    const int arms = 6;
    float angleStep = (3.14159265f * 2.0f) / arms;

    for (int i = 0; i < arms; i++) {
        float angle = spiralAngle + angleStep * i;
        float vx = cosf(angle) * 4.0f;
        float vy = sinf(angle) * 4.0f;

        new EnemyBullet2(x + 32, y + 32, vx, vy, 8.0f, 0);
    }
}

void Boss3::SpawnDiagonalBeam()
{
    for (int i = 0; i < 2; i++) {
        if (!diagonalBeams[i].active) {
            diagonalBeams[i].active = true;
            diagonalBeams[i].timer = 120.0f;

            if (i == 0) {
                diagonalBeams[i].startX = Field::STAGE_LEFT;
                diagonalBeams[i].startY = Field::STAGE_TOP;
                diagonalBeams[i].angle = 3.14159265f / 4.0f;
            }
            else {
                diagonalBeams[i].startX = Field::STAGE_RIGHT;
                diagonalBeams[i].startY = Field::STAGE_TOP;
                diagonalBeams[i].angle = 3.14159265f * 3.0f / 4.0f;
            }
            break;
        }
    }
}

void Boss3::UpdateDiagonalBeams()
{
    for (int i = 0; i < 2; i++) {
        if (diagonalBeams[i].active) {
            diagonalBeams[i].timer -= 1.0f;
            if (diagonalBeams[i].timer <= 0.0f) {
                diagonalBeams[i].active = false;
            }
        }
    }
}

void Boss3::ShotBullet(float angle, float num)
{
    // 使用しない（各フェーズで個別処理）
}

void Boss3::Draw()
{
    if (isActive && bossImage != -1) {
        if (spawnInvincible) {
            if (((int)(spawnInvincibleTimer / 5)) % 2 == 0)
            {
                DrawGraph((int)x, (int)y, bossImage, TRUE);
            }
        }
        else {
            DrawGraph((int)x, (int)y, bossImage, TRUE);
        }

        if (bulletPhase == BulletPhase3::PHASE_2) {
            DrawBeams();
        }

        if (bulletPhase == BulletPhase3::PHASE_4) {
            DrawDiagonalBeams();
        }

        SetFontSize(40);
        DrawFormatString(930, 200, GetColor(255, 0, 0), "BOSS HP: %d", currentHp);
        DrawFormatString(920, 150, GetColor(255, 0, 0), "=== BOSS3 ===");
    }
}

void Boss3::DrawBeams()
{
    for (int i = 0; i < 4; i++) {
        if (beams[i].isWarning) {
            int alpha = (int)(255 * (beams[i].warningTimer / 60.0f));
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

            float endX = (x + 32) + cosf(beams[i].angle) * 1500.0f;
            float endY = (y + 32) + sinf(beams[i].angle) * 1500.0f;

            DrawLine((int)(x + 32), (int)(y + 32), (int)endX, (int)endY, GetColor(255, 0, 0), 3);

            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
        else if (beams[i].isActive) {
            float endX = (x + 32) + cosf(beams[i].angle) * 1500.0f;
            float endY = (y + 32) + sinf(beams[i].angle) * 1500.0f;

            for (int w = -15; w <= 15; w += 5) {
                float perpX = -sinf(beams[i].angle) * w;
                float perpY = cosf(beams[i].angle) * w;

                DrawLine(
                    (int)(x + 32 + perpX), (int)(y + 32 + perpY),
                    (int)(endX + perpX), (int)(endY + perpY),
                    GetColor(100, 150, 255), 2
                );
            }
        }
    }
}

void Boss3::DrawDiagonalBeams()
{
    for (int i = 0; i < 2; i++) {
        if (diagonalBeams[i].active) {
            float beamLength = 1500.0f;
            float endX = diagonalBeams[i].startX + cosf(diagonalBeams[i].angle) * beamLength;
            float endY = diagonalBeams[i].startY + sinf(diagonalBeams[i].angle) * beamLength;

            for (int w = -15; w <= 15; w += 5) {
                float perpX = -sinf(diagonalBeams[i].angle) * w;
                float perpY = cosf(diagonalBeams[i].angle) * w;

                DrawLine(
                    (int)(diagonalBeams[i].startX + perpX),
                    (int)(diagonalBeams[i].startY + perpY),
                    (int)(endX + perpX),
                    (int)(endY + perpY),
                    GetColor(100, 150, 255), 2
                );
            }
        }
    }
}

void Boss3::TakeDamage(int dmg)
{
    if (spawnInvincible) return;

    currentHp -= dmg;
    if (currentHp <= 0) {
        currentHp = 0;
        isDead = true;
    }
}

bool Boss3::IsHit(float bx, float by, int rad)
{
    if (!isActive) return false;

    float dx = bx - (x + 60);
    float dy = by - (y + 60);
    float d = sqrt(dx * dx + dy * dy);

    if (d < 60 + rad) {
        TakeDamage(74);
        return true;
    }
    return false;
}

float Boss3::GetHpPercent() const
{
    if (maxHp == 0) return 0.0f;
    return (float)currentHp / (float)maxHp;
}

int Boss3::GetCurrentPhaseNumber() const
{
    return (int)bulletPhase + 1;
}