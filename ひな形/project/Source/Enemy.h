#pragma once

class Enemy {  // GameObjectŒp³‚ğíœ
protected:
    float x, y;
    float vx, vy;
    int hp;
    bool isDead;
    int hImage;
public:
    Enemy();
    Enemy(float sx, float sy, float svx, float svy, int health);
    virtual ~Enemy();
    virtual void Update();      // override‚ğíœ
    virtual void Draw();        // override‚ğíœ
    virtual void TakeDamage(int dmg);
    bool IsDead() const { return isDead || hp <= 0; }
    bool IsOutOfScreen() const;
    float GetX() const { return x; }
    float GetY() const { return y; }
    int GetHP() const { return hp; }
};