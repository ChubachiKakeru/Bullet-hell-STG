#pragma once
#include"Field.h"
#include "../Library/GameObject.h"

class Player;
class Bulett;

// ========================================
// 列挙型
// ========================================

// 弾幕フェーズ
enum class BulletPhase2 {
    PHASE_1,    // フェーズ1：8方向弾
    PHASE_2,    // フェーズ2：左右移動 + 弾幕
    PHASE_3     // フェーズ3：左右移動 + チャージ + プレイヤー狙い
};


enum class BossPattern2 {
    CIRCLE,       // ?~?O??
    FIGURE_EIGHT, // ?????
    LEFT_RIGHT    // ???E???
};


class Boss2 : public GameObject {
public:
    // ========================================
    // コンストラクタ・デストラクタ
    // ========================================
    Boss2();
    Boss2(int sx, int sy);
    ~Boss2();

    // ========================================
    // 基本メソッド
    // ========================================
    void Update() override;
    void Draw() override;

    // ========================================
    // HP関連
    // ========================================
    void TakeDamage(int damage);
    bool IsAlive() const { return currentHp > 0; }
    int GetCurrentHp() const { return currentHp; }
    int GetMaxHp() const { return maxHp; }
    float GetHpPercent() const;

    // ========================================
    // 当たり判定
    // ========================================
    bool IsHit(float bx, float by, int rad);

    // ========================================
    // 位置取得
    // ========================================
    float GetX() const { return x; }
    float GetY() const { return y; }
    float GetSize() const { return size; }

    // ========================================
    // フェーズ関連
    // ========================================
    BulletPhase2 GetBulletPhase() const { return bulletPhase2; }
    int GetCurrentPhaseNumber() const;
    bool IsCharging() const { return isCharging; }

    // ========================================
    // 弾発射
    // ========================================
    void ShotBullet(float rad, float num);
    void ShootBullet();  // プレイヤー狙い弾

private:
    // ========================================
    // 定数
    // ========================================
    static constexpr float DegToRad = 3.14159265f / 180.0f;
    static constexpr float PI = 3.14159265f;

    // ========================================
    // グラフィック
    // ========================================
    int hImage;

    // ========================================
    // 位置
    // ========================================
    float x, y;
    float centerX, centerY;
    float size;

    // ========================================
    // HP管理
    // ========================================
    int maxHp;          // 最大HP
    int currentHp;      // 現在のHP
    int Phase2Hp;       // フェーズ2に移行するHP閾値
    int Phase3Hp;       // フェーズ3に移行するHP閾値

    // ========================================
    // フェーズ管理
    // ========================================
    BulletPhase2 bulletPhase2;      // 現在のフェーズ
    BulletPhase2 previousPhase2;    // 前回のフェーズ（変化検知用）

    // ========================================
    // 弾発射管理
    // ========================================
    float shotTimer;        // 弾発射タイマー
    float shotInterval;     // 弾発射間隔

    // ========================================
    // 移動管理（フェーズ2,3用）
    // ========================================
    float moveDirection;    // 移動速度兼方向

    // ========================================
    // チャージ管理（フェーズ3用）
    // ========================================
    bool isCharging;        // チャージ中フラグ
    float chargeTimer;      // チャージタイマー

    // ========================================
    // 状態フラグ
    // ========================================
    bool isActive;

    // ========================================
    // 非公開メソッド
    // ========================================

    // フェーズ管理
    void CheckPhaseTransition();              // HPベースでフェーズをチェック
    void OnPhaseChanged(int newPhase);        // フェーズ変更時の処理

    // フェーズ別更新
    void UpdatePhase1();                      // フェーズ1の更新
    void UpdatePhase2();                      // フェーズ2の更新
    void UpdatePhase3();                      // フェーズ3の更新
};