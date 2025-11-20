#include "Bullet.h"
#include "DxLib.h"
#include"Screen.h"
#include"Boss1.h"

Bullet::Bullet() : GameObject()
{
    hImage = LoadGraph("data/image/yaiba.png");
    x = 0;
    y = 0;
    velocityX = 0;
    velocityY = 0;
    isActive = true;

    // デフォルトの判定サイズ（弾は小さめ）
    rectWidth = 16.0f;      // 矩形: 16x16
    rectHeight = 16.0f;
    circleRadius = 4.0f;    // 円形: 半径4
}

Bullet::Bullet(int sx, int sy, float vx, float vy) : GameObject()
{
    hImage = LoadGraph("data/image/yaiba.png");
    x = sx;
    y = sy;
    velocityX = vx;
    velocityY = vy;
    isActive = true;

    rectWidth = 16.0f;
    rectHeight = 16.0f;
    circleRadius = 4.0f;
}

Bullet::~Bullet()
{
    if (hImage != -1) {
        DeleteGraph(hImage);
    }
}

void Bullet::Update() {
    if (!isActive) return;

    // 弾の移動
    x += velocityX;
    y += velocityY;
    Boss1* b = FindGameObject<Boss1>();
    b->IsHit(x+40,y+40,40);

    // 画面外に出たら無効化
    if (x < -32 || x > 840 + 32|| y < -32 || y > Screen::HEIGHT) {
        isActive = false;
    }
}

void Bullet::Draw() {
    if (isActive && hImage != -1) {
        DrawGraph((int)x, (int)y, hImage, TRUE);
    }
    DrawCircle(x, y, 60, GetColor(255, 255, 255), FALSE);
}