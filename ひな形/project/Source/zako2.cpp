#include "zako2.h"
#include "Field.h"
#include "enemyBullet.h"
#include "Player.h"
#include "BackGround.h"
#include <cmath>

namespace {
    constexpr float SHOT_INTERVAL = 120.0f;  // 2秒（60FPS想定）
    constexpr float PATTERN_TIME = 300.0f;   // 5秒で消える
    constexpr float RADIUS = 40.0f;
    constexpr float S_AMPLITUDE = 120.0f;   // 横振れ幅
    constexpr float S_FREQUENCY = 0.03f;    // S字の細かさ
    constexpr float VERTICAL_SPEED = -1.2f; // 上方向へ進む
    constexpr int BURST_INTERVAL = 12;      // 連射の間隔（0.2秒）
    constexpr int BURST_COUNT = 4;           // 4連射
    constexpr int SHOT_WAIT = 240;           // 4秒ごとに攻撃
}

zako2::zako2(float sx, float sy, Zako2Pattern pat)
{
    zakoImage = LoadGraph("data/image/zako2.png");
    x = sx;
    y = sy;
    pattern = pat;
    moveTimer = 0;
    shotTimer = 0;
    shotInterval = SHOT_INTERVAL;
    moveSpeed = 0.0f;
    isActive = true;
    patternComplete = false;

    hp = 1;
    isDead = false;

    // パターンに応じて移動速度を設定
    switch (pattern) {
    case Zako2Pattern::PATTERN_LEFT_TO_RIGHT:
        moveSpeed = 1.0f;  // 右へ
        break;
    case Zako2Pattern::PATTERN_RIGHT_TO_LEFT:
        moveSpeed = -1.0f;  // 左へ
        break;
    }

    pattern = pat;

    baseX = sx;
    sAmplitude = 120.0f;
    sFrequency = 0.03f;

    if (pattern == Zako2Pattern::PATTERN_S_UP)
        verticalSpeed = -1.2f;   // 上へ
    else if (pattern == Zako2Pattern::PATTERN_S_DOWN)
        verticalSpeed = 1.2f;    // 下へ
}

zako2::~zako2()
{
    if (zakoImage != -1) {
        DeleteGraph(zakoImage);
        zakoImage = -1;
    }
}

void zako2::Update()
{
    if (!isActive) {
        isDead = true;
        return;
    }

    moveTimer++;
    shotTimer++;

    // 横方向に移動（パターンに応じて左右）
    x += moveSpeed;

    // 弾発射（2秒ごと）
    if (shotTimer >= shotInterval) {
        ShootBullet();
        shotTimer = 0;
    }

    // ---- 攻撃管理 ----
    if (!isBurst)
    {
        if (shotTimer >= shotInterval)
        {
            isBurst = true;
            burstCount = 0;
            burstTimer = 0;
            shotTimer = 0;
        }
    }
    else
    {
        burstTimer++;
        if (burstTimer >= BURST_INTERVAL)
        {
            ShootBullet(); // 既存の関数を使う
            burstTimer = 0;
            burstCount++;

            if (burstCount >= BURST_COUNT)
            {
                isBurst = false;
            }
        }
    }

    if (pattern == Zako2Pattern::PATTERN_S_UP ||
        pattern == Zako2Pattern::PATTERN_S_DOWN)
    {
        moveTimer++;

        y += verticalSpeed;
        x = baseX + sinf(moveTimer * sFrequency) * sAmplitude;
    }

    // 画面外判定（画面外に出たら削除）
    if (x < Field::STAGE_LEFT - 70.0f || x > Field::STAGE_RIGHT + 70.0f) {
        isActive = false;
        isDead = true;
        //DestroyMe();
    }
}

void zako2::ShootBullet()
{
    // プレイヤーを探す
    Player* player = FindGameObject<Player>();
    if (!player) return;

    // プレイヤーへの角度を計算
    float dx = player->GetX() - x;
    float dy = player->GetY() - y;
    float angle = atan2f(dy, dx);

    // 敵弾を生成（プレイヤー狙い）
    new enemyBullet(x, y, angle, 3.0f);
}

bool zako2::IsHit(float bx, float by, int rad)
{
    if (!isActive || isDead) return false;

    float dx = x - bx;
    float dy = y - by;
    float distance = sqrtf(dx * dx + dy * dy);

    if (distance < RADIUS + rad) {
        hp--;
        if (hp <= 0) {
            isDead = true;
            isActive = false;
        }
        return true;
    }
    return false;
}

void zako2::Draw()
{
    if (isActive && zakoImage != -1) {
        // 100x100の画像の中心をx, yに合わせる
        DrawGraph((int)(x - 50), (int)(y - 50), zakoImage, TRUE);
    }

    // デバッグ用：当たり判定を表示
#ifdef _DEBUG
    if (isActive) {
        // 当たり判定の円（赤）
        DrawCircle((int)x, (int)y, (int)RADIUS, GetColor(255, 0, 0), FALSE);
        // 中心点（黄色）
        DrawCircle((int)x, (int)y, 3, GetColor(255, 255, 0), TRUE);
        // 画像の矩形範囲（緑）
        DrawBox((int)(x - 50), (int)(y - 50),
            (int)(x + 50), (int)(y + 50),
            GetColor(0, 255, 0), FALSE);
    }
#endif
}
