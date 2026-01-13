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
    // Boss1と同じスタイルで本体内で初期化
    zakoImage = LoadGraph("data/image/zako2.jpg");

    x = sx;
    y = sy;

    pattern = pat;
    moveTimer = 0;
    shotTimer = 0;
    shotInterval = SHOT_INTERVAL;
    moveSpeed = 0.0f;

    isActive = true;
    patternComplete = false;

    // 基底クラスの変数も初期化
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

}

void zako2::ShootBullet()
{

}

void zako2::Draw()
{
    if (isActive && zakoImage != -1) {
        // 100x100の画像の中心をx, yに合わせる
        DrawGraph((int)(x - 50), (int)(y - 50), zakoImage, TRUE);
    }

    // デバッグ用：当たり判定を表示
    if (isActive) {
        // 当たり判定の円（赤）
        DrawCircle((int)x, (int)y, (int)RADIUS, GetColor(255, 0, 0), FALSE);

        // 中心点（黄色）
        DrawCircle((int)x, (int)y, 3, GetColor(255, 255, 0), TRUE);

        // 画像の矩形範囲（緑）- デバッグ用
        DrawBox((int)(x - 50), (int)(y - 50),
            (int)(x + 50), (int)(y + 50),
            GetColor(0, 255, 0), FALSE);
    }
}