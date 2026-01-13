#pragma once
#include "Field.h"
#include "Enemy.h"

class Player;
class BackGround;

// 雑魚敵の移動パターン
enum class Zako2Pattern {
    PATTERN_LEFT_TO_RIGHT,   // 左から右へ
    PATTERN_RIGHT_TO_LEFT    // 右から左へ
};

class zako2 : public Enemy
{
public:
    zako2(float sx, float sy, Zako2Pattern pat = Zako2Pattern::PATTERN_LEFT_TO_RIGHT);
    virtual ~zako2();

    void Update() override;
    void Draw() override;

    bool IsActive() const { return isActive; }
    int GetHP() const { return hp; }
    float GetX() const { return x; }
    float GetY() const { return y; }
    bool IsPatternComplete() const { return patternComplete; }

    bool IsHit(float bx, float by, int rad);

private:
    int zakoImage;
    bool isActive;
    bool patternComplete;
    Zako2Pattern pattern;
    float moveTimer;
    float shotTimer;
    float shotInterval;
    float moveSpeed;  // 横移動速度

    void ShootBullet();
};