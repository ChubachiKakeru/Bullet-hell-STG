#include "zako1.h"
#include "Field.h"
#include "enemyBullet.h"
#include "Player.h"
#include "BackGround.h"
#include <cmath>

namespace {
    constexpr float SHOT_INTERVAL = 120.0f;  // 2秒（60FPS想定）
    constexpr float PATTERN_TIME = 300.0f;   // 5秒で消える
    constexpr float RADIUS = 20.0f;
}

zako1::zako1(float sx, float sy, Zako1Pattern pat) : Enemy(),
zakoImage(LoadGraph("data/image/file/chara/enemy.png")),
isActive(true), patternComplete(false),
pattern(pat), moveTimer(0), shotTimer(0),
shotInterval(SHOT_INTERVAL), moveSpeed(0.0f)
{
    x = sx;
    y = sy;
    hp = 1;
    isDead = false;

    // パターンに応じて移動速度を設定
    switch (pattern) {
    case Zako1Pattern::PATTERN_LEFT_TO_RIGHT:
        moveSpeed = 1.0f;  // 右へ（3.0fから1.0fに変更）
        break;
    case Zako1Pattern::PATTERN_RIGHT_TO_LEFT:
        moveSpeed = -1.0f;  // 左へ（-3.0fから-1.0fに変更）
        break;
    }
}

zako1::~zako1()
{
    if (zakoImage != -1) {
        DeleteGraph(zakoImage);
        zakoImage = -1;
    }
}

void zako1::Update()
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

void zako1::ShootBullet()
{
    // 真下に弾を発射
    new enemyBullet(x, y, 0, 5, 8);
}

void zako1::Draw()
{
    if (isActive && zakoImage != -1) {
        DrawGraph((int)(x - 16), (int)(y - 16), zakoImage, TRUE);
    }

    // デバッグ用：当たり判定を表示
    if (isActive) {
        DrawCircle((int)x, (int)y, (int)RADIUS, GetColor(255, 0, 0), FALSE);
    }
}

bool zako1::IsHit(float bx, float by, int rad)
{
    if (!isActive) return false;

    float dx = bx - x;
    float dy = by - y;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance < RADIUS + rad) {
        // 当たったら無効化（EnemyManagerが削除する）
        isActive = false;
        isDead = true;
        return true;
    }
    return false;
}