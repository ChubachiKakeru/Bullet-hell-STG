#include "Enemy.h"
#include"Screen.h"

Enemy::Enemy()
{
    x = 0.0f;
    y = 0.0f;
    vx = 0.0f;
    vy = 0.0f;
    hp = 1;
    isDead = false;
    hImage = -1;
}

Enemy::Enemy(float sx, float sy, float vx, float vy, int health)
{
    x = sx;
    y = sy;
    vx = 0.0f;
    vy = 0.0f;
    hp = health;
    isDead = false;
    hImage = -1;
}

Enemy::~Enemy()
{
    if (hImage != -1) {
        DeleteGraph(hImage);
        hImage = -1;
    }
}

void Enemy::Update() {
    // 派生クラスでオーバーライド
}

void Enemy::Draw() {
    // 派生クラスでオーバーライド
}

void Enemy::TakeDamage(int dmg) {
    hp -= dmg;
    if (hp <= 0) {
        isDead = true;
    }
}

bool Enemy::IsOutOfScreen() const {
    return (y > Screen::HEIGHT || y < -100 || x < -100 || x > Screen::WIDTH);
}