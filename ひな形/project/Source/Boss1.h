#pragma once
#include"Field.h"
#include "../Library/GameObject.h"

class Player;
class Bulett;

enum class BossPattern {
    CIRCLE,       // 円軌道
    FIGURE_EIGHT, // 八の字
    LEFT_RIGHT    // 左右移動
};

// 弾幕フェーズの列挙型
enum class BulletPhase {
    PHASE_1,    // 1段階目：八角形の角から連続発射
    PHASE_2,    // 2段階目：扇形イメージ + 左右移動
    PHASE_3     // 3段階目：2段階目と同じ + 1秒溜め
};

class Boss1 : public GameObject
{
public:
    Boss1();
    Boss1(int sx, int sy);
    ~Boss1();
    void Update()override;
    void Draw() override;

    int GetHP() const { return hp; }
    void TakeDamage(int damage);
    bool IsHit(float bx, float by, int rad);
    bool IsAlive() const { return isActive; }

    BulletPhase GetBulletPhase() const;
    bool IsCharging() const;

    float GetX() const { return x; }
    float GetY() const { return y; }
    float GetCenterX() const { return x; }
    float GetCenterY() const { return y; }
    float GetSize() const { return size; }
    void ShotBullet(float rad, float num);

    bool ShouldFireBullet();

private:
    int hImage;
    float x, y;
    float centerX, centerY;
    int hp;
    bool isActive;
    float size;

    // 判定サイズ
    float rectWidth;
    float rectHeight;
    float circleRadius;

    // 移動パターン関連
    BossPattern pattern;
    float moveTimer;
    float patternChangeTime;
    float patternTimer;

    // 弾幕フェーズ関連
    BulletPhase bulletPhase;
    float phaseTimer;
    float phaseChangeTime;
    float bulletFireTimer;
    float bulletFireInterval;

    // 円軌道用
    float radius;
    float angle;
    float angularSpeed;

    // 八の字用
    float figureEightScale;

    // 左右移動用
    float speed;
    int direction;

    // フェーズ固有の変数
    float octagonAngle;      // 八角形の角度
	float fanAngle;          // 扇形の中心角度
    float horizontalSpeed;   // 左右移動速度
    float moveDirection;     // 移動方向（1: 右, -1: 左）
    float chargeTimer;       // 溜め時間
    bool isCharging;         // 溜め中フラグ

    // 弾発射関連
    float shotTimer;
    float shotInterval;

    void UpdateCircle();
    void UpdateFigureEight();
    void UpdateLeftRight();
    void ChangePattern();
    void ShootBullet();
    void ShootFanDown();

    // フェーズ更新関数
    void UpdatePhase1();
    void UpdatePhase2();
    void UpdatePhase3();

    // フェーズ変更
    void ChangeBulletPhase();
};

