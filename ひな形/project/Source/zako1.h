#pragma once
#include "Field.h"
#include "Enemy.h"

class Player;
class BackGround;

// 雑魚敵の移動パターン
enum class Zako1Pattern {
    PATTERN_LEFT_TO_RIGHT,   // 左から右へ
    PATTERN_RIGHT_TO_LEFT    // 右から左へ
};

class zako1 : public Enemy
{
public:
    zako1(float sx, float sy, Zako1Pattern pat = Zako1Pattern::PATTERN_LEFT_TO_RIGHT);
    virtual ~zako1();

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
    Zako1Pattern pattern;
    float moveTimer;
    float shotTimer;
    float shotInterval;
    float moveSpeed;  // 横移動速度

    void ShootBullet();

    // サウンドパス
    const char* GAME_EDEATH_SOUND_PATH = "data/Sound/enemydeath.mp3";      //エネミーDeathジングル
    //  サウンドハンドル
    int enemyDeathSoundHandle;		//	エネミーDeathサウンドハンドル
};