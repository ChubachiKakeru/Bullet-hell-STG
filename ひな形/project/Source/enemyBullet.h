#pragma once
#include"Bullet.h"
class enemyBullet : public Bullet
{
public:
    // コンストラクタ
    enemyBullet(float sx, float sy, float vx, float vy, float bulletsize = 8.0f);

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

