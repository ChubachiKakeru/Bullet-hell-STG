#pragma once
#include "Enemy.h"

enum class BulletPhase3
{
    PHASE_1 = 0,  // バラマキ+自機追尾弾
    PHASE_2 = 1,  // ビーム攻撃
    PHASE_3 = 2,  // らせん→反回転
    PHASE_4 = 3   // バラマキ+追尾+斜めビーム
};

class Boss3 : public Enemy
{
private:
    int bossImage;

    float centerX, centerY;
    float size;

    int maxHp;
    int currentHp;
    int Phase2Hp;  // フェーズ2移行HP (225)
    int Phase3Hp;  // フェーズ3移行HP (150)
    int Phase4Hp;  // フェーズ4移行HP (75)

    BulletPhase3 bulletPhase;
    BulletPhase3 previousPhase;

    float shotTimer;
    float shotInterval;

    float moveDirection;

    bool isCharging;
    float chargeTimer;

    bool isActive;

    // ★出現時無敵★
    bool spawnInvincible;
    float spawnInvincibleTimer;

    // フェーズ1用タイマー
    float scatterShotTimer;
    float scatterShotInterval;
    float aimedShotTimer;
    float aimedShotInterval;

    // フェーズ2用（ビーム）
    struct BeamData {
        float angle;          // ビームの角度
        float warningTimer;   // 予告タイマー
        float activeTimer;    // 発射タイマー
        bool isActive;        // 発射中かどうか
        bool isWarning;       // 予告中かどうか
    };
    BeamData beams[4];        // 最大4本のビーム
    int activeBeamCount;
    float beamSpawnTimer;
    float beamSpawnInterval;

    // フェーズ3用（らせん）
    float spiralAngle;
    float spiralSpeed;
    bool spiralReversed;
    float spiralTimer;

    // フェーズ4用
    float diagonalBeamTimer;
    float diagonalBeamInterval;
    struct DiagonalBeam {
        float startX, startY;
        float angle;
        float timer;
        bool active;
    };
    DiagonalBeam diagonalBeams[2];

public:
    Boss3();
    Boss3(float sx, float sy);
    ~Boss3();

    void Update() override;
    void Draw() override;
    bool IsHit(float bx, float by, int rad);
    void TakeDamage(int dmg);

    float GetHpPercent() const;
    int GetCurrentPhaseNumber() const;

private:
    void CheckPhaseTransition();
    void OnPhaseChanged(int newPhase);

    void UpdatePhase1();
    void UpdatePhase2();
    void UpdatePhase3();
    void UpdatePhase4();

    void ShotBullet(float angle, float num);
    void ShootScatterBullets();   // 小さい白玉バラマキ
    void ShootAimedBullets();     // 大きい白玉自機追尾

    void InitBeams();
    void UpdateBeams();
    void DrawBeams();
    void SpawnBeam();

    void ShootSpiralBullets();

    void UpdateDiagonalBeams();
    void DrawDiagonalBeams();
    void SpawnDiagonalBeam();
};