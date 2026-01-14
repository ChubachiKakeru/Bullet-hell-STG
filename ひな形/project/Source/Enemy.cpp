#include "Enemy.h"
#include "Screen.h"
#include"../Library/myDxLib.h"

Enemy::Enemy()  // GameObject()の呼び出しを削除
{
    x = 0.0f;
    y = 0.0f;
    vx = 0.0f;
    vy = 0.0f;
    hp = 1;
    isDead = false;
    hImage = -1;
}

Enemy::Enemy(float sx, float sy, float svx, float svy, int health)  // GameObject()の呼び出しを削除
{
    x = sx;
    y = sy;
    vx = svx;
    vy = svy;
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