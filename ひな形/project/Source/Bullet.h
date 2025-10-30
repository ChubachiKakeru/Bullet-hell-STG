#pragma once
#include "../Library/GameObject.h"

class Bullet : public GameObject
{
public:
    Bullet();
    Bullet(int sx, int sy, float vx, float vy);
    ~Bullet();

    void Update() override;
    void Draw() override;

    bool IsActive() const { return isActive; }
    float GetX() const { return x; }
    float GetY() const { return y; }

private:
    int hImage;
    float x, y;
    float velocityX, velocityY;
    bool isActive;
};

