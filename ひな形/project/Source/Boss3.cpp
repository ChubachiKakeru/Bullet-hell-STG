#include "Boss3.h"
#include "Field.h"
#include "EnemyBullet3.h"
#include "Player.h"
#include "../Library/GameObject.h"
#include <cmath>
#include <cstdlib>

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
    beamWaveType = 0;
    beamOffsetX = 0.0f;
    InitBeams();

    spiralAngle = 0.0f;
    spiralSpeed = 0.01047f;
    spiralReversed = false;
    spiralTimer = 0.0f;

    phase4BarrageTimer = 0.0f;
    phase4BarrageInterval = 45.0f;
    phase4AimedTimer = 0.0f;
    phase4AimedInterval = 240.0f;

    for (int i = 0; i < 3; i++) {
        waitingBullets[i].active = false;
    }

    diagonalBeamTimer = 0.0f;
    diagonalBeamInterval = 180.0f;
    for (int i = 0; i < 4; i++) {  // 4本分に拡張
        diagonalBeams[i].active = false;
        diagonalBeams[i].timer = 0.0f;
        diagonalBeams[i].warningTimer = 0.0f;
    }

    sideObjTimer = 0.0f;
    sideObjMoveSpeed = 2.0f;
    sideObjDirection = 1.0f;
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
    beamWaveType = 0;
    beamOffsetX = 0.0f;
    InitBeams();

    spiralAngle = 0.0f;
    spiralSpeed = 0.08f;
    spiralReversed = false;
    spiralTimer = 0.0f;

    phase4BarrageTimer = 0.0f;
    phase4BarrageInterval = 45.0f;
    phase4AimedTimer = 0.0f;
    phase4AimedInterval = 240.0f;

    diagonalBeamTimer = 0.0f;
    diagonalBeamInterval = 180.0f;
    for (int i = 0; i < 4; i++) {  // 4本分に拡張
        diagonalBeams[i].active = false;
        diagonalBeams[i].timer = 0.0f;
        diagonalBeams[i].warningTimer = 0.0f;
    }

    for (int i = 0; i < 3; i++) {
        waitingBullets[i].active = false;
    }

    sideObjTimer = 0.0f;
    sideObjMoveSpeed = 2.0f;
    sideObjDirection = 1.0f;
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

    sideObjTimer += sideObjMoveSpeed * sideObjDirection;
    if (sideObjTimer >= 100.0f) {
        sideObjDirection = -1.0f;
    }
    else if (sideObjTimer <= 0.0f) {
        sideObjDirection = 1.0f;
    }

    // サイドオブジェクトからの追尾弾発射（全フェーズで120フレーム間隔）
   /* static float sideObjShotTimer = 0.0f;
    sideObjShotTimer += 1.0f;
    if (sideObjShotTimer >= 120.0f) {
        ShootSideObjectBullets();
        sideObjShotTimer = 0.0f;
    }*/

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
        x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 100.0f;  // 画像サイズ200に合わせて調整
        y = Field::STAGE_TOP + 20.0f;
        scatterShotTimer = 0.0f;
        aimedShotTimer = 0.0f;
        break;

    case BulletPhase3::PHASE_2:
        x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 100.0f;  // 画像サイズ200に合わせて調整
        y = Field::STAGE_TOP + 20.0f;
        beamOffsetX = 0.0f;
        InitBeams();
        beamSpawnTimer = 0.0f;
        break;

    case BulletPhase3::PHASE_3:
        x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 100.0f;  // 画像サイズ200に合わせて調整
        y = Field::STAGE_TOP + 20.0f;
        spiralAngle = 0.0f;
        spiralSpeed = 0.01047f;
        spiralReversed = false;
        spiralTimer = 0.0f;
        shotTimer = 0.0f;
        shotInterval = 15.0f;  // 発射間隔を短縮（20→15フレーム）
        break;

    case BulletPhase3::PHASE_4:
        x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 100.0f;  // 画像サイズ200に合わせて調整
        y = Field::STAGE_TOP + 20.0f;
        scatterShotTimer = 0.0f;
        aimedShotTimer = 0.0f;
        diagonalBeamTimer = 0.0f;
        diagonalBeamInterval = 180.0f;
        phase4BarrageTimer = 0.0f;
        phase4AimedTimer = 0.0f;
        for (int i = 0; i < 3; i++) {
            waitingBullets[i].active = false;
        }
        for (int i = 0; i < 4; i++) {  // 4本分に拡張
            diagonalBeams[i].active = false;
            diagonalBeams[i].timer = 0.0f;
            diagonalBeams[i].warningTimer = 0.0f;
        }
        break;
    }
}

void Boss3::InitBeams()
{
    for (int i = 0; i < 9; i++) {
        beams[i].isActive = false;
        beams[i].isWarning = false;
        beams[i].warningTimer = 0.0f;
        beams[i].activeTimer = 0.0f;
        beams[i].x = 0.0f;
        beams[i].y = 0.0f;
        beams[i].angle = 0.0f;
    }
    activeBeamCount = 0;
}

void Boss3::UpdatePhase1()
{
    x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 100.0f;  // 画像サイズ200に合わせて調整
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
    x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 100.0f;  // 画像サイズ200に合わせて調整
    y = Field::STAGE_TOP + 20.0f;

    beamSpawnTimer += 1.0f;
    if (beamSpawnTimer >= beamSpawnInterval) {
        SpawnBeam();
        beamSpawnTimer = 0.0f;
        beamWaveType = (beamWaveType + 1) % 2;
    }

    UpdateBeams();

    Player* player = FindGameObject<Player>();
    if (player) {
        float playerCenterX = player->GetX() + 75.0f;
        float playerCenterY = player->GetY() + 75.0f;

        for (int i = 0; i < 9; i++) {
            if (beams[i].isActive) {
                const float beamWidth = 75.0f;

                if (playerCenterX >= beams[i].x - beamWidth / 2.0f &&
                    playerCenterX <= beams[i].x + beamWidth / 2.0f &&
                    playerCenterY >= Field::STAGE_TOP &&
                    playerCenterY <= Field::STAGE_TOP + 1280) {
                    player->TakeDamage(1);
                }
            }
        }
    }
}

void Boss3::UpdatePhase3()
{
    x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 100.0f;  // 画像サイズ200に合わせて調整
    y = Field::STAGE_TOP + 20.0f;

    spiralTimer += 1.0f;

    if (spiralTimer >= 300.0f) {
        spiralSpeed = -spiralSpeed;
        spiralTimer = 0.0f;
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
    x = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f - 100.0f;  // 画像サイズ200に合わせて調整
    y = Field::STAGE_TOP + 20.0f;

    phase4BarrageTimer += 1.0f;
    phase4AimedTimer += 1.0f;
    diagonalBeamTimer += 1.0f;

    if (phase4BarrageTimer >= phase4BarrageInterval) {
        ShootScatterBullets();
        phase4BarrageTimer = 0.0f;
    }

    if (phase4AimedTimer >= phase4AimedInterval) {
        SpawnWaitingBullet();
        phase4AimedTimer = 0.0f;
    }

    if (diagonalBeamTimer >= diagonalBeamInterval) {
        SpawnDiagonalBeam();
        diagonalBeamTimer = 0.0f;
    }

    UpdateWaitingBullets();
    UpdateDiagonalBeams();

    // ×字ビームとプレイヤーの当たり判定
    Player* player = FindGameObject<Player>();
    if (player) {
        float playerCenterX = player->GetX() + 75.0f;
        float playerCenterY = player->GetY() + 75.0f;

        for (int i = 0; i < 4; i++) {  // 4本分に拡張
            if (diagonalBeams[i].active) {
                const float beamWidth = 80.0f;

                // ビームの始点と角度から、プレイヤーまでの距離を計算
                float dx = playerCenterX - diagonalBeams[i].startX;
                float dy = playerCenterY - diagonalBeams[i].startY;

                // ビーム方向のベクトル
                float beamDirX = cosf(diagonalBeams[i].angle);
                float beamDirY = sinf(diagonalBeams[i].angle);

                // プレイヤーのビーム上での位置（内積）
                float t = dx * beamDirX + dy * beamDirY;

                // ビーム上の最も近い点
                float closestX = diagonalBeams[i].startX + beamDirX * t;
                float closestY = diagonalBeams[i].startY + beamDirY * t;

                // プレイヤーとビームの距離
                float distX = playerCenterX - closestX;
                float distY = playerCenterY - closestY;
                float distance = sqrt(distX * distX + distY * distY);

                // ビームの範囲内かつ幅内ならヒット（両方向に判定）
                if (t >= -2000.0f && t <= 2000.0f && distance < beamWidth / 2.0f) {
                    player->TakeDamage(1);
                }
            }
        }
    }
}

void Boss3::ShootScatterBullets()
{
    const int bulletCount = 10;
    float baseAngle = 3.14159265f / 2.0f;
    float spreadAngle = 1.2f;

    for (int i = 0; i < bulletCount; i++) {
        float t = (float)i / (bulletCount - 1);
        float angle = baseAngle - spreadAngle * 0.5f + spreadAngle * t;

        float vx = cosf(angle) * 4.0f;
        float vy = sinf(angle) * 4.0f;

        // フェーズ1では反射弾を使用しない
        if (bulletPhase == BulletPhase3::PHASE_1) {
            new EnemyBullet3(x + 100, y + 100, vx, vy, 6.0f, false);  // 画像サイズ200の中心
        }
        else {
            // フェーズ4では10%の確率で反射弾
            bool enableReflect = (rand() % 10 == 0);
            new EnemyBullet3(x + 100, y + 100, vx, vy, 6.0f, enableReflect);  // 画像サイズ200の中心
        }
    }
}

void Boss3::ShootAimedBullets()
{
    Player* player = FindGameObject<Player>();
    if (!player) return;

    float dx = player->GetX() - x;
    float dy = player->GetY() - y;
    float angle = atan2f(dy, dx);

    float speed = 3.0f;
    float vx = cosf(angle) * speed;
    float vy = sinf(angle) * speed;

    // 追尾弾は反射しない
    new EnemyBullet3(x + 100, y + 100, vx, vy, 20.0f);  // 画像サイズ200の中心
}

void Boss3::SpawnBeam()
{
    InitBeams();

    // ビームを打つたびに左右に移動（±50ピクセル）
    if (beamWaveType == 0) {
        beamOffsetX += 50.0f;
        if (beamOffsetX > 200.0f) beamOffsetX = 200.0f;
    }
    else {
        beamOffsetX -= 50.0f;
        if (beamOffsetX < -200.0f) beamOffsetX = -200.0f;
    }

    if (beamWaveType == 0) {
        float screenWidth = 840.0f;
        float centerX = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f;  // 画面中央
        float spacing = screenWidth / 6.0f;

        for (int i = 0; i < 5; i++) {
            beams[i].isWarning = true;
            beams[i].warningTimer = 60.0f;
            // 中央を基準にして配置し、オフセットを適用
            beams[i].x = centerX + spacing * (i - 2) + beamOffsetX;  // i-2で中央基準に
            beams[i].y = Field::STAGE_TOP + 1280.0f;
            beams[i].angle = -3.14159265f / 2.0f;
        }
        activeBeamCount = 5;
    }
    else {
        float screenWidth = 840.0f;
        float centerX = (Field::STAGE_LEFT + Field::STAGE_RIGHT) / 2.0f;  // 画面中央
        float spacing = screenWidth / 6.0f;

        float delays[5] = { 0.0f, 15.0f, 30.0f, 15.0f, 0.0f };

        for (int i = 0; i < 5; i++) {
            beams[i].isWarning = true;
            beams[i].warningTimer = 60.0f + delays[i];
            // 中央を基準にして配置し、オフセットを適用
            beams[i].x = centerX + spacing * (i - 2) + beamOffsetX;  // i-2で中央基準に
            beams[i].y = Field::STAGE_TOP + 1280.0f;
            beams[i].angle = -3.14159265f / 2.0f;
        }
        activeBeamCount = 5;
    }
}

void Boss3::UpdateBeams()
{
    for (int i = 0; i < 9; i++) {
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
            }
        }
    }
}

void Boss3::ShootSpiralBullets()
{
    const int arms = 8;  // 螺旋の本数を8本に変更
    float angleStep = (3.14159265f * 2.0f) / arms;

    // 画像サイズ200x200の中心
    float bossCenterX = x + 100.0f;
    float bossCenterY = y + 100.0f;

    for (int i = 0; i < arms; i++) {
        float angle = spiralAngle + angleStep * i;

        float speed = 4.0f;
        float vx = cosf(angle) * speed;
        float vy = sinf(angle) * speed;

        float spawnX = bossCenterX;
        float spawnY = bossCenterY;

        // 螺旋弾幕は全て発射（画面外で削除される）
        new EnemyBullet3(spawnX, spawnY, vx, vy, 10.0f);
    }
}

//void Boss3::ShootSideObjectBullets()
//{
//    Player* player = FindGameObject<Player>();
//    if (!player) return;
//
//    // 左右のサイドオブジェクトの位置を計算
//    float offsetY = sideObjTimer;
//    float leftSideX = x - 100;
//    float leftSideY = y + offsetY + 50;  // 画像の中心付近から発射
//    float rightSideX = x + 180;
//    float rightSideY = y + offsetY + 50;
//
//    // プレイヤーの位置
//    float playerX = player->GetX();
//    float playerY = player->GetY();
//
//    // 左側サイドオブジェクトから追尾弾
//    float leftDx = playerX - leftSideX;
//    float leftDy = playerY - leftSideY;
//    float leftAngle = atan2f(leftDy, leftDx);
//    float leftSpeed = 3.5f;
//    float leftVx = cosf(leftAngle) * leftSpeed;
//    float leftVy = sinf(leftAngle) * leftSpeed;
//    new EnemyBullet3(leftSideX, leftSideY, leftVx, leftVy, 20.0f);
//
//    // 右側サイドオブジェクトから追尾弾
//    float rightDx = playerX - rightSideX;
//    float rightDy = playerY - rightSideY;
//    float rightAngle = atan2f(rightDy, rightDx);
//    float rightSpeed = 3.5f;
//    float rightVx = cosf(rightAngle) * rightSpeed;
//    float rightVy = sinf(rightAngle) * rightSpeed;
//    new EnemyBullet3(rightSideX, rightSideY, rightVx, rightVy, 20.0f);
//}

void Boss3::SpawnWaitingBullet()
{
    Player* player = FindGameObject<Player>();
    if (!player) return;

    for (int i = 0; i < 3; i++) {
        if (!waitingBullets[i].active) {
            waitingBullets[i].active = true;
            waitingBullets[i].x = x + 100;  // 画像サイズ200の中心
            waitingBullets[i].y = y + 100;  // 画像サイズ200の中心
            waitingBullets[i].targetX = player->GetX();
            waitingBullets[i].targetY = player->GetY();
            waitingBullets[i].waitTimer = 120.0f;
            break;
        }
    }
}

void Boss3::UpdateWaitingBullets()
{
    Player* player = FindGameObject<Player>();

    for (int i = 0; i < 3; i++) {
        if (waitingBullets[i].active) {
            waitingBullets[i].waitTimer -= 1.0f;

            if (waitingBullets[i].waitTimer <= 0.0f) {
                if (player) {
                    float dx = player->GetX() - waitingBullets[i].x;
                    float dy = player->GetY() - waitingBullets[i].y;
                    float angle = atan2f(dy, dx);

                    float speed = 5.0f;
                    float vx = cosf(angle) * speed;
                    float vy = sinf(angle) * speed;

                    // 待機弾は反射しない
                    new EnemyBullet3(
                        waitingBullets[i].x,
                        waitingBullets[i].y,
                        vx, vy,
                        20.0f
                    );
                }

                waitingBullets[i].active = false;
            }
        }
    }
}

void Boss3::DrawWaitingBullets()
{
    for (int i = 0; i < 3; i++) {
        if (waitingBullets[i].active) {
            float blinkRate = waitingBullets[i].waitTimer / 120.0f;

            if (((int)(waitingBullets[i].waitTimer / 10)) % 2 == 0) {
                int alpha = (int)(200 * blinkRate);
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
                DrawCircle(
                    (int)waitingBullets[i].x,
                    (int)waitingBullets[i].y,
                    20,
                    GetColor(255, 50, 50),
                    TRUE
                );
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

                DrawCircle(
                    (int)waitingBullets[i].x,
                    (int)waitingBullets[i].y,
                    20,
                    GetColor(255, 200, 0),
                    FALSE
                );
            }

            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
            DrawLine(
                (int)waitingBullets[i].x,
                (int)waitingBullets[i].y,
                (int)waitingBullets[i].targetX,
                (int)waitingBullets[i].targetY,
                GetColor(255, 100, 100),
                1
            );
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
    }
}

void Boss3::SpawnDiagonalBeam()
{
    Player* player = FindGameObject<Player>();
    if (!player) return;

    // プレイヤーの中心座標を取得
    float playerCenterX = player->GetX() + 75.0f;
    float playerCenterY = player->GetY() + 75.0f;

    // 画面上部の左側の発射位置
    float leftStartX = Field::STAGE_LEFT + 100.0f;
    float leftStartY = Field::STAGE_TOP;

    // 画面上部の右側の発射位置
    float rightStartX = Field::STAGE_RIGHT - 100.0f;
    float rightStartY = Field::STAGE_TOP;

    // 左側からプレイヤーへの角度を計算
    float leftDx = playerCenterX - leftStartX;
    float leftDy = playerCenterY - leftStartY;
    float leftAngleToPlayer = atan2f(leftDy, leftDx);

    // 右側からプレイヤーへの角度を計算
    float rightDx = playerCenterX - rightStartX;
    float rightDy = playerCenterY - rightStartY;
    float rightAngleToPlayer = atan2f(rightDy, rightDx);

    // 1本目: 左からプレイヤーの方向
    diagonalBeams[0].active = true;
    diagonalBeams[0].timer = 120.0f;
    diagonalBeams[0].warningTimer = 60.0f;
    diagonalBeams[0].startX = leftStartX;
    diagonalBeams[0].startY = leftStartY;
    diagonalBeams[0].angle = leftAngleToPlayer;

    // 2本目: 右からプレイヤーの方向
    diagonalBeams[1].active = true;
    diagonalBeams[1].timer = 120.0f;
    diagonalBeams[1].warningTimer = 60.0f;
    diagonalBeams[1].startX = rightStartX;
    diagonalBeams[1].startY = rightStartY;
    diagonalBeams[1].angle = rightAngleToPlayer;

    // 3本目、4本目は使用しない
    diagonalBeams[2].active = false;
    diagonalBeams[3].active = false;
}

void Boss3::UpdateDiagonalBeams()
{
    for (int i = 0; i < 4; i++) {  // 4本分に拡張
        if (diagonalBeams[i].active) {
            // 予測線タイマーを更新
            if (diagonalBeams[i].warningTimer > 0.0f) {
                diagonalBeams[i].warningTimer -= 1.0f;
            }

            diagonalBeams[i].timer -= 1.0f;
            if (diagonalBeams[i].timer <= 0.0f) {
                diagonalBeams[i].active = false;
            }
        }
    }
}

void Boss3::ShotBullet(float angle, float num)
{
    // 使用しない
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

        int sideImage = LoadGraph("data/image/fire.png");
        if (sideImage != -1) {
            float offsetY = sideObjTimer;
            DrawGraph((int)(x - 100), (int)(y + offsetY), sideImage, TRUE);
            DrawGraph((int)(x + 180), (int)(y + offsetY), sideImage, TRUE);
            DeleteGraph(sideImage);
        }

        if (bulletPhase == BulletPhase3::PHASE_2) {
            DrawBeams();
        }

        if (bulletPhase == BulletPhase3::PHASE_4) {
            DrawDiagonalBeams();
            DrawWaitingBullets();
        }

        SetFontSize(40);
        DrawFormatString(930, 200, GetColor(255, 0, 0), "BOSS HP: %d", currentHp);
        DrawFormatString(920, 150, GetColor(255, 0, 0), "=== BOSS3 ===");
    }
}

void Boss3::DrawBeams()
{
    const float beamWidth = 75.0f;

    for (int i = 0; i < 9; i++) {
        if (beams[i].isWarning) {
            int alpha = (int)(255 * (beams[i].warningTimer / 60.0f));
            if (((int)beams[i].warningTimer / 10) % 2 == 0) {
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

                DrawLine((int)beams[i].x, (int)Field::STAGE_TOP,
                    (int)beams[i].x, (int)(Field::STAGE_TOP + 1280),
                    GetColor(255, 0, 0), 3);

                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            }
        }
        else if (beams[i].isActive) {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);

            DrawBox((int)(beams[i].x - beamWidth / 2), (int)Field::STAGE_TOP,
                (int)(beams[i].x + beamWidth / 2), (int)(Field::STAGE_TOP + 1280),
                GetColor(100, 150, 255), TRUE);

            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

            DrawLine((int)(beams[i].x - beamWidth / 2), (int)Field::STAGE_TOP,
                (int)(beams[i].x - beamWidth / 2), (int)(Field::STAGE_TOP + 1280),
                GetColor(255, 200, 0), 2);
            DrawLine((int)(beams[i].x + beamWidth / 2), (int)Field::STAGE_TOP,
                (int)(beams[i].x + beamWidth / 2), (int)(Field::STAGE_TOP + 1280),
                GetColor(255, 200, 0), 2);
        }
    }
}

void Boss3::DrawDiagonalBeams()
{
    for (int i = 0; i < 4; i++) {  // 4本分に拡張
        if (diagonalBeams[i].active) {
            float beamLength = 2000.0f;
            float beamWidth = 80.0f;

            // ビームを両方向に延長（ボスから前後に伸びる）
            float endX1 = diagonalBeams[i].startX + cosf(diagonalBeams[i].angle) * beamLength;
            float endY1 = diagonalBeams[i].startY + sinf(diagonalBeams[i].angle) * beamLength;
            float endX2 = diagonalBeams[i].startX - cosf(diagonalBeams[i].angle) * beamLength;
            float endY2 = diagonalBeams[i].startY - sinf(diagonalBeams[i].angle) * beamLength;

            // 予測線（警告表示）
            if (diagonalBeams[i].warningTimer > 0.0f) {
                int alpha = (int)(200 * (diagonalBeams[i].warningTimer / 60.0f));
                if (((int)(diagonalBeams[i].warningTimer / 10)) % 2 == 0) {
                    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

                    // 赤い予測線を描画
                    DrawLine(
                        (int)endX2,
                        (int)endY2,
                        (int)endX1,
                        (int)endY1,
                        GetColor(255, 0, 0), 3
                    );

                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
                }
            }
            // 実際のビーム（予測線が消えた後）
            else {
                // ビーム本体（青い半透明）
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, 160);

                for (int offset = -40; offset <= 40; offset += 3) {
                    float perpX = -sinf(diagonalBeams[i].angle) * offset;
                    float perpY = cosf(diagonalBeams[i].angle) * offset;

                    // 両方向に描画
                    DrawLine(
                        (int)(endX2 + perpX),
                        (int)(endY2 + perpY),
                        (int)(endX1 + perpX),
                        (int)(endY1 + perpY),
                        GetColor(100, 150, 255), 2
                    );
                }
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

                // 縁取り（両端の黄色いライン）
                float edge1X = -sinf(diagonalBeams[i].angle) * (beamWidth / 2.0f);
                float edge1Y = cosf(diagonalBeams[i].angle) * (beamWidth / 2.0f);
                float edge2X = -sinf(diagonalBeams[i].angle) * (-beamWidth / 2.0f);
                float edge2Y = cosf(diagonalBeams[i].angle) * (-beamWidth / 2.0f);

                DrawLine(
                    (int)(endX2 + edge1X),
                    (int)(endY2 + edge1Y),
                    (int)(endX1 + edge1X),
                    (int)(endY1 + edge1Y),
                    GetColor(255, 200, 0), 4
                );
                DrawLine(
                    (int)(endX2 + edge2X),
                    (int)(endY2 + edge2Y),
                    (int)(endX1 + edge2X),
                    (int)(endY1 + edge2Y),
                    GetColor(255, 200, 0), 4
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

    // ボス画像の実際の中心に合わせて当たり判定を調整
    // 画像サイズ200x200の中心（x+100, y+100）
    float bossCenterX = x + 100.0f;
    float bossCenterY = y + 100.0f;

    float dx = bx - bossCenterX;
    float dy = by - bossCenterY;
    float d = sqrt(dx * dx + dy * dy);

    // 当たり判定の半径を80に調整（画像サイズ200x200に合わせて）
    if (d < 80.0f + rad) {
        TakeDamage(5);
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