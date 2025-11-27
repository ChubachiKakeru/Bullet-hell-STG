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

    // íeÇÃà⁄ìÆ
    x += velocityX;
    y += velocityY;
    Boss1* b = FindGameObject<Boss1>();
    b->IsHit(x+40,y+40,40);

    // âÊñ äOÇ…èoÇΩÇÁñ≥å¯âª
    if (x < -32 || x > 840 + 32|| y < -32 || y > Screen::HEIGHT) {
        isActive = false;
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
