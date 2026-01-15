#include "zako1.h"
#include "Field.h"
#include "enemyBullet.h"
#include "Player.h"
#include "BackGround.h"
#include <cmath>
#include"../Library/myDxLib.h"

namespace {
    constexpr float SHOT_INTERVAL = 60.0f*1.5f;  // 2秒（60FPS想定）
    constexpr float PATTERN_TIME = 300.0f;   // 5秒で消える
    constexpr float RADIUS = 40.0f;
}

zako1::zako1(float sx, float sy, Zako1Pattern pat) : Enemy()
{
    // Boss1と同じスタイルで本体内で初期化
    zakoImage = LoadGraph("data/image/zako1.png");

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
    case Zako1Pattern::PATTERN_LEFT_TO_RIGHT:
        moveSpeed = 1.0f;  // 右へ
        break;
    case Zako1Pattern::PATTERN_RIGHT_TO_LEFT:
        moveSpeed = -1.0f;  // 左へ
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
        //DestroyMe();
    }
}

void zako1::ShootBullet()
{
    // 真下に弾を発射（画像の中心から）
    new enemyBullet(x, y, 0, 5, 8);
}
void zako1::Draw()
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

    }
}
bool zako1::IsHit(float bx, float by, int rad)
{
    if (!isActive) return false;

    float dx = bx - x ;
    float dy = by - y ;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance < RADIUS + rad) {
        // 当たったら無効化（EnemyManagerが削除する）
        PlaySoundFile(GAME_EDEATH_SOUND_PATH, DX_PLAYTYPE_BACK);
        isActive = false;
        isDead = true;
        return true;
    }
    return false;
}

