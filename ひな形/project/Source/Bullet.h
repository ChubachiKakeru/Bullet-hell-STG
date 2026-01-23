#pragma once
#include "../Library/GameObject.h"

class Bullet : public GameObject
{
public:
    Bullet();
    Bullet(float sx, float sy, float vx, float vy,float bulletsize);
    ~Bullet();

    void Update() override;
    void Draw() override;

    bool IsActive() const { return isActive; }
    float GetX() const { return x; }
    float GetY() const { return y; }

    bool IsAlive() const { return isAlive; }
    float GetCenterX() const { return x; }
    float GetCenterY() const { return y; }
    float GetSize() const { return size; }

    void Deactivate() { isActive = false; }
    void Hit();

protected:
    int hImage;
    float x, y;
    float velocityX, velocityY;
    float size;
    bool isActive;
    bool isAlive;
    // îªíËÉTÉCÉY
    float rectWidth;
    float rectHeight;
    float circleRadius;
};

