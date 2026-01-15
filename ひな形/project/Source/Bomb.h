#pragma once
#include "../Library/GameObject.h"

class Bomb : public GameObject
{
public:
    Bomb(float sx, float sy, float vx, float vy, float bombsize);
    ~Bomb();
    void Update() override;
    void Draw() override;

    float GetX() const { return x; }
    float GetY() const { return y; }
    float GetSize() const { return size; }
    bool IsActive() const { return isActive; }

private:
    int hImage;
    float x, y;
    float velocityX, velocityY;
    float size;
    bool isActive;
};