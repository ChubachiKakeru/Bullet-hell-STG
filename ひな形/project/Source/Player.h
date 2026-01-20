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
    void ShootBomb();        // ★ボム発射用
    void AddBomb(int num) { bombCount += num; }
    int GetBombCount() const { return bombCount; }
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
    // ボム関連
    int bombCount;
    float bombSize;      // 150固定
    float bombSpeed;     // 弾としての移動速度
    bool prevBombKeyPressed;  // ボムキーの前フレーム状態（連続発射防止用）
    // サウンドパス
    const char* GAME_PHIT_SOUND_PATH = "data/Sound/playerhit.mp3";      //プレイヤー死亡ジングル
    const char* GAME_PSHOT_SOUND_PATH = "data/Sound/playerattack.mp3";      //自機弾発射ジングル
    const char* GAME_PBOMB_SOUND_PATH = "data/Sound/bombattack.mp3";      //ボム発射ジングル
    //  サウンドハンドル
    int playerHitSoundHandle;		//	プレイヤーHitサウンドハンドル
    int playerShotSoundHandle;         // プレイヤーShotサウンドハンドル
    int playerBombSoundHandle;         // プレイヤーBombサウンドハンドル
};