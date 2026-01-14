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

    // 画面外判定（画面外に出たら削除）
    if (x < Field::STAGE_LEFT - 100.0f || x > Field::STAGE_RIGHT + 100.0f) {
        isActive = false;
        isDead = true;
        DestroyMe();
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