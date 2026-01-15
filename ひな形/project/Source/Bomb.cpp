#include "Bomb.h"
#include "enemyBullet.h"
#include "EnemyBullet2.h"
#include "Field.h"
#include <cmath>

Bomb::Bomb(float sx, float sy, float vx, float vy, float bombsize)
    : GameObject()
{
    hImage = LoadGraph("data/image/bomb.png");
    x = sx;
    y = sy;
    velocityX = vx;
    velocityY = vy;
    size = bombsize;
    isActive = true;
}

Bomb::~Bomb()
{
    if (hImage != -1) {
        DeleteGraph(hImage);
        hImage = -1;
    }
}

void Bomb::Update()
{
    if (!isActive) return;

    // 移動
    x += velocityX;
    y += velocityY;

    // ボムの中心位置
    float bombCenterX = x + size / 2.0f;
    float bombCenterY = y + size / 2.0f;
    float bombRadius = size / 2.0f;

    // 敵弾との当たり判定
    auto bullets = FindGameObjects<enemyBullet>();
    for (auto* bullet : bullets) {
        if (bullet && bullet->IsActive()) {
            float bulletCenterX = bullet->GetX() + 38.0f; // 76x83の中心
            float bulletCenterY = bullet->GetY() + 41.5f;

            float dx = bombCenterX - bulletCenterX;
            float dy = bombCenterY - bulletCenterY;
            float distance = sqrt(dx * dx + dy * dy);

            // ボムと敵弾が接触したら敵弾を消す
            if (distance < bombRadius + 40.0f) {
                bullet->DestroyMe();
            }
        }
    }

    // EnemyBullet2との当たり判定
    auto bullets2 = FindGameObjects<EnemyBullet2>();
    for (auto* bullet : bullets2) {
        if (bullet && bullet->IsActive()) {
            float bulletCenterX = bullet->GetX() + 38.0f;
            float bulletCenterY = bullet->GetY() + 41.5f;

            float dx = bombCenterX - bulletCenterX;
            float dy = bombCenterY - bulletCenterY;
            float distance = sqrt(dx * dx + dy * dy);

            if (distance < bombRadius + 40.0f) {
                bullet->DestroyMe();
            }
        }
    }

    // 画面外判定
    if (y < -200) {
        DestroyMe();
    }
}

void Bomb::Draw()
{
    if (isActive && hImage != -1) {
        DrawGraph((int)x, (int)y, hImage, TRUE);
        // デバッグ用：ボムの当たり判定円を表示
        DrawCircle((int)(x + size / 2.0f), (int)(y + size / 2.0f),
            (int)(size / 2.0f), GetColor(0, 255, 0), FALSE);
    }
}