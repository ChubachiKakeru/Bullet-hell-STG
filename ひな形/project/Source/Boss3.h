#pragma once
#include "Enemy.h"

enum class BulletPhase3 {
    PHASE_1 = 0,  // バラマキ + 追尾弾
    PHASE_2 = 1,  // 縦ビーム
    PHASE_3 = 2,  // 回転螺旋弾
    PHASE_4 = 3   // 複合攻撃（バラマキ + 待機弾 + ×字ビーム）
};

struct Beam {
    bool isActive;
    bool isWarning;
    float warningTimer;
    float activeTimer;
    float x;
    float y;
    float angle;
};

struct DiagonalBeam {
    bool active;
    float timer;
    float startX;
    float startY;
    float angle;
};

struct WaitingBullet {
    bool active;
    float x;
    float y;
    float targetX;
    float targetY;
    float waitTimer;
};

class Boss3 : public Enemy
{
private:
    int bossImage;

    // 基本パラメータ
    float centerX;
    float centerY;
    float size;

    // HP管理
    int maxHp;
    int currentHp;
    int Phase2Hp;
    int Phase3Hp;
    int Phase4Hp;

    // フェーズ管理
    BulletPhase3 bulletPhase;
    BulletPhase3 previousPhase;

    // 移動・攻撃タイマー
    float shotTimer;
    float shotInterval;
    float moveDirection;

    // チャージ攻撃用
    bool isCharging;
    float chargeTimer;

    // アクティブフラグ
    bool isActive;

    // 無敵時間
    bool spawnInvincible;
    float spawnInvincibleTimer;

    // フェーズ1用
    float scatterShotTimer;
    float scatterShotInterval;
    float aimedShotTimer;
    float aimedShotInterval;

    // フェーズ2用（ビーム）
    Beam beams[9];
    int activeBeamCount;
    float beamSpawnTimer;
    float beamSpawnInterval;
    int beamWaveType;

    // フェーズ3用（螺旋弾）
    float spiralAngle;
    float spiralSpeed;
    bool spiralReversed;
    float spiralTimer;

    // フェーズ4用
    float phase4BarrageTimer;
    float phase4BarrageInterval;
    float phase4AimedTimer;
    float phase4AimedInterval;
    float diagonalBeamTimer;
    float diagonalBeamInterval;
    WaitingBullet waitingBullets[3];
    DiagonalBeam diagonalBeams[2];

    // サイドオブジェクト用（左右の追尾弾画像）
    float sideObjTimer;
    float sideObjMoveSpeed;
    float sideObjDirection;

public:
    Boss3();
    Boss3(float sx, float sy);
    ~Boss3();

    void Update() override;
    void Draw() override;
    void TakeDamage(int dmg) override;
    bool IsHit(float bx, float by, int rad);

    float GetHpPercent() const;
    int GetCurrentPhaseNumber() const;

private:
    // フェーズ管理
    void CheckPhaseTransition();
    void OnPhaseChanged(int newPhase);

    // 各フェーズの更新処理
    void UpdatePhase1();
    void UpdatePhase2();
    void UpdatePhase3();
    void UpdatePhase4();

    // フェーズ1の攻撃
    void ShootScatterBullets();
    void ShootAimedBullets();

    // フェーズ2の攻撃（ビーム）
    void InitBeams();
    void SpawnBeam();
    void UpdateBeams();
    void DrawBeams();

    // フェーズ3の攻撃（螺旋弾）
    void ShootSpiralBullets();

    // フェーズ4の攻撃
    void SpawnWaitingBullet();
    void UpdateWaitingBullets();
    void DrawWaitingBullets();
    void SpawnDiagonalBeam();
    void UpdateDiagonalBeams();
    void DrawDiagonalBeams();

    // 未使用（継承用）
    void ShotBullet(float angle, float num);
};