#pragma once
#include"Bullet.h"
class playerBullet : public Bullet
{
public:
    // コンストラクタ
    playerBullet(float sx, float sy, float vx, float vy, float bulletsize = 5.0f);

    // オーバーライド
    void Update() override;
    void Draw() override;
private:
    float x, y;
    int hImage;
    float velocityX, velocityY;
    float size;
    bool isActive;
    bool isAlive;
};

