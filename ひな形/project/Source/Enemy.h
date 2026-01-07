#pragma once
#include "../Library/GameObject.h"

class Enemy : public GameObject {
protected:  // 修正: private → protected (派生クラスからアクセス可能にする)
    float x, y;
    float vx, vy;
    int hp;
    bool isDead;  // 修正: int → bool
    int hImage;

public:
    Enemy();
    Enemy(float sx, float sy, float svx, float svy, int health);  // 引数名を明確化
    virtual ~Enemy();  // virtualを追加

    virtual void Update() override;
    virtual void Draw() override;
    virtual void TakeDamage(int dmg);

    bool IsDead() const { return isDead || hp <= 0; }
    bool IsOutOfScreen() const;

    float GetX() const { return x; }
    float GetY() const { return y; }
    int GetHP() const { return hp; }
};