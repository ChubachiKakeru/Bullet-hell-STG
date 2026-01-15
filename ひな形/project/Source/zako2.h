#pragma once
#include "Field.h"
#include "Enemy.h"

class Player;
class BackGround;

// 雑魚敵の移動パターン
enum class Zako2Pattern {
    PATTERN_LEFT_TO_RIGHT,   // 左から右へ
    PATTERN_RIGHT_TO_LEFT,    // 右から左へ
    PATTERN_S_UP,     // 下→上 S字（フェーズ3）
    PATTERN_S_DOWN    // 上→下 S字（フェーズ4）
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

    float baseX;        // S字の中心X
    float baseY;        // 開始Y
    float sAmplitude;   // 横振れ幅
    float sFrequency;   // S字の周期
    float verticalSpeed;// 上昇速度

    // --- 4連射制御 ---
    int burstCount;     // 今何発目か
    int burstTimer;     // 次の弾までの間隔
    bool isBurst;       // 連射中かどうか

    void ShootBullet();

    // サウンドパス
    const char* GAME_EDEATH_SOUND_PATH = "data/Sound/enemydeath.mp3";      //エネミーDeathジングル
    //  サウンドハンドル
    int enemyDeathSoundHandle;		//	エネミーDeathサウンドハンドル
};