#include "Bullet.h"
#include "DxLib.h"
#include"Screen.h"

Bullet::Bullet() : GameObject()
{
    hImage = LoadGraph("data/image/stone.png");
    x = 0;
    y = 0;
    velocityX = 0;
    velocityY = 0;
    isActive = true;
}

Bullet::Bullet(int sx, int sy, float vx, float vy) : GameObject()
{
    hImage = LoadGraph("data/image/stone.png");
    x = sx;
    y = sy;
    velocityX = vx;
    velocityY = vy;
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

    // âÊñ äOÇ…èoÇΩÇÁñ≥å¯âª
    if (x < -32 || x > 640 + 32 || y < -32 || y > Screen::HEIGHT) {
        isActive = false;
    }
}

void Bullet::Draw() {
    if (isActive && hImage != -1) {
        DrawGraph((int)x, (int)y, hImage, TRUE);
    }
}