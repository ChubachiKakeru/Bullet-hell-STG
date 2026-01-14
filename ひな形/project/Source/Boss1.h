#pragma once
#include "Field.h"
#include "Enemy.h"

class Player;
class Bulett;

// ========================================
// 列挙型
// ========================================

// 弾幕フェーズ
enum class BulletPhase {
    PHASE_1,
    PHASE_2,
    PHASE_3
};

enum class BossPattern {
    CIRCLE,
    FIGURE_EIGHT,
    LEFT_RIGHT
};

class Boss1 : public Enemy {
public:
    Boss1();
    Boss1(float sx, float sy);
    virtual ~Boss1();

    void Update() override;
    void Draw() override;

    // HP関連
    void TakeDamage(int damage);
    bool IsAlive() const { return isActive && currentHp > 0; }
    int GetCurrentHp() const { return currentHp; }
    int GetMaxHp() const { return maxHp; }
    float GetHpPercent() const;

    // 当たり判定
    bool IsHit(float bx, float by, int rad);

    // 位置取得
    float GetX() const { return x; }
    float GetY() const { return y; }
    float GetSize() const { return size; }

    // フェーズ関連
    BulletPhase GetBulletPhase() const { return bulletPhase; }
    int GetCurrentPhaseNumber() const;
    bool IsCharging() const { return isCharging; }

    // 弾発射
    void ShotBullet(float rad, float num);
    void ShootBullet();

    // ================================
    // 無敵関連 (追加)
    // ================================
    bool IsInvincible() const { return spawnInvincible; }

private:
    static constexpr float DegToRad = 3.14159265f / 180.0f;
    static constexpr float PI = 3.14159265f;

    // 無敵時間 (1秒)
    static constexpr float INVINCIBLE_DURATION = 1.0f;

    int bossImage;
    float centerX, centerY;
    float size;

    // HP管理
    int maxHp;
    int currentHp;
    int Phase2Hp;
    int Phase3Hp;

    // フェーズ管理
    BulletPhase bulletPhase;
    BulletPhase previousPhase;

    // 弾発射管理
    float shotTimer;
    float shotInterval;

    // 移動管理
    float moveDirection;

    // チャージ管理
    bool isCharging;
    float chargeTimer;

    bool isActive;

    // ================================
    // 無敵状態管理 (追加)
    // ================================
    bool spawnInvincible;
    float spawnInvincibleTimer;

    // 非公開メソッド
    void CheckPhaseTransition();
    void OnPhaseChanged(int newPhase);

    void UpdatePhase1();
    void UpdatePhase2();
    void UpdatePhase3();
};
