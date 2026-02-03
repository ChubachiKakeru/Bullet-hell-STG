#pragma once
#include "../Library/GameObject.h"
#include <vector>

class Field;
class enemyBullet;
class EnemyBullet2;
class Bomb;

class Player : public GameObject
{
public:
    Player();
    Player(int sx, int sy);
    ~Player();
    void Initialize(float startX, float startY, Field* fieldPtr);
    void Update() override;
    void Draw() override;
    float GetX() const { return x; }
    float GetY() const { return y; }
    bool IsAlive() const { return isActive; }
    float GetCenterX() const { return x; }
    float GetCenterY() const { return y; }
    float GetSize() const { return size; }
    void TakeDamage(int damage);
    int GetHP() const { return hp; }
    bool IsHit(float bx, float by, int rad);
    bool IsActive() const { return isActive; }
    void ShootBullet();
    void ShootBomb();
    //  void AddBomb(int num) { bombCount += num; }
    int GetBombCount() const { return bombCount; }
    bool IsInvincible() const { return invincibleTimer > 0; }  // ★追加★
    static constexpr float MOVE_SPEED = 7.0f;
    static constexpr float SHOT_COOLDOWN = 50.0f;
    static  constexpr int INITIAL_HP = 10;
    static constexpr float PLAYER_COLLISION_RADIUS = 30.0f;
    static constexpr float PLAYER_CENTER_OFFSET = 60.0f;
    static constexpr float BULLET_SPEED = -10.0f;
    static constexpr float BULLET_RADIUS = 8.0f;

    static constexpr float BOMB_SIZE = 150.0f;
    static constexpr float BOMB_SPEED = -10.0f;
    int bombCount;

    int GetCurrentHp() const { return hp; }
    void SetCurrentHp(int newHp) { hp = (newHp > maxHp) ? maxHp : newHp; }

    int GetCurrentBomb() const { return bombCount; }
    void SetCurrentBomb(int newBomb) { bombCount = (newBomb > s_initialBombCount) ? s_initialBombCount : newBomb; }

    int GetMaxHP() const { return maxHp; }  // ★追加★
    // ★ショップ用の関数（現在のプレイヤーインスタンスに適用）★
    void AddHP(int amount);          // HP回復
    // 最大HPは10が上限なのでAddMaxHPで増やすときは必ず上限をチェック
    void AddMaxHP(int amount);
    void AddBomb(int amount);
    static void UpgradeMaxHP(int amount);
    static void UpgradeInitialBombCount(int amount);
    static void ResetUpgrades();                     // 強化をリセット
    // ★静的変数のゲッター★
    static int GetStaticMaxHP() { return s_maxHp; }
    static int GetStaticInitialBombCount() { return s_initialBombCount; }

    // ステージ開始時の初期化
    void ResetStatus(bool inherit, int prevHp = 0, int prevBomb = 0);

private:
    int hImage;
    int hBombImage;
    float x, y;
    float velocity;
    bool onGround;
    Field* field;
    float shotTimer;
    float size;
    int hp;
    bool isActive;
    int maxHp;

    // ボム関連
   // int bombCount;
    float bombSize;
    float bombSpeed;
    bool prevBombKeyPressed;

    // ★無敵時間関連を追加★
    int invincibleTimer;
    static constexpr int INVINCIBLE_DURATION = 120;  // 無敵時間（2秒 = 120フレーム）

    // サウンドパス
    const char* GAME_PHIT_SOUND_PATH = "data/Sound/playerhit.mp3";
    const char* GAME_PSHOT_SOUND_PATH = "data/Sound/playerattack.mp3";
    const char* GAME_PBOMB_SOUND_PATH = "data/Sound/bombattack.mp3";

    // サウンドハンドル
    int playerHitSoundHandle;
    int playerShotSoundHandle;
    int playerBombSoundHandle;

    static int s_maxHp;              // 最大HP（ショップで強化可能）
    static int s_initialBombCount;   // 初期ボム数（ショップで強化可能）
};