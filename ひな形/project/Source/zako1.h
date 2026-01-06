#pragma once
#include "Field.h"
#include "Enemy.h"

class Player;

// 雑魚敵の移動パターン（1→2→3の順で進行）
enum class Zako1Pattern {
    PATTERN_1,    // 左から横並び、真下に弾
    PATTERN_2,    // 右から横並び、真下に弾
    PATTERN_3     // 左右移動、自機狙い5way弾
};

class zako1 : public Enemy
{
public:
    zako1(int sx, int sy, Zako1Pattern pat = Zako1Pattern::PATTERN_1);
    ~zako1();

    void Update() override;
    void Draw() override;

    bool IsActive() const { return isActive; }
    int GetHP() const { return hp; }
    float GetX() const { return x; }
    float GetY() const { return y; }
    bool IsPatternComplete() const { return patternComplete; }

    void TakeDamage(int damage);
    bool IsHit(float bx, float by, int rad);

private:
    int hImage;
    float x, y;
    int hp;
    bool isActive;
    bool patternComplete;

    Zako1Pattern pattern;
    float moveTimer;
    float shotTimer;
    int movePhase;  // パターン3の左右移動フェーズ

    void ShootBullet();
};