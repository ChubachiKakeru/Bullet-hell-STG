#include "Enemy.h"
#include "Screen.h"

Enemy::Enemy() : GameObject()  // 追加: GameObject()を呼び出す
{
    x = 0.0f;
    y = 0.0f;
    vx = 0.0f;
    vy = 0.0f;
    hp = 1;
    isDead = false;
    hImage = -1;
}

Enemy::Enemy(float sx, float sy, float svx, float svy, int health) : GameObject()  // 追加: GameObject()を呼び出す、引数名修正
{
    x = sx;
    y = sy;
    vx = svx;  // 修正: 引数の値を正しく設定
    vy = svy;  // 修正: 引数の値を正しく設定
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