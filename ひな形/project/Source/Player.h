#pragma once
#include"../Library/GameObject.h"

class Field;
class Bulett;

class Player : public GameObject
{
public:
    Player();
    Player(int sx, int sy);
    ~Player();

    void Initialize(float startX, float startY, Field* fieldPtr);
    void Update() override;
    void Draw() override;

    float GetX() const { return x; }
    float GetY() const { return y; }

    void ShootBullet();

private:
    int hImage;
    float x, y;
    float velocity;
    bool onGround;
    Field* field;
    float shotTimer;     // òAéÀêßå¿óp
};

