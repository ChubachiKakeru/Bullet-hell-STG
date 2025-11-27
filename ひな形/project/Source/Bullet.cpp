#include "Bullet.h"
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


   
     //デフォルトの判定サイズ（弾は小さめ）
    rectWidth = 16.0f;      // 矩形: 16x16
    rectHeight = 16.0f;
    circleRadius = 4.0f;    // 円形: 半径4
}

Bullet::Bullet(float sx, float sy, float vx, float vy, float bulletsize) : GameObject()
{
    hImage = LoadGraph("data/image/yaiba.png");
    x = sx;
    y = sy;
    velocityX = vx;
    velocityY = vy;
    size = bulletsize;
    isActive = true;
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

    // 画面外に出たら無効化
    if (x < -32 || x > 840 + 32|| y < -32 || y > Screen::HEIGHT) {
        DestroyMe();
    }
}

void Bullet::Draw() {
    if (isActive && hImage != -1) {
        DrawGraph((int)x, (int)y, hImage, TRUE);
    }
    DrawCircle(x+40, y+40, 60, GetColor(255, 255, 255), FALSE);
}

void Bullet::Hit()
{
    isAlive = false;
}
