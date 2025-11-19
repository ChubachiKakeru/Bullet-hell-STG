#pragma once
#include "../Library/GameObject.h"
#include <vector>

class Player;
class Boss1;
class Bullet;

class Collision : public GameObject
{
public:
    Collision();
    ~Collision();

    void Update() override;
    void Draw() override {}  // 描画なし

    // シングルトン
    static Collision* GetInstance() { return instance; }

    // 弾配列へのアクセス
    std::vector<Bullet*>& GetPlayerBullets() { return playerBullets; }
    std::vector<Bullet*>& GetBossBullets() { return bossBullets; }

private:
    static Collision* instance;

    std::vector<Bullet*> playerBullets;
    std::vector<Bullet*> bossBullets;

    // 自作判定関数（★ここを実装★）
    bool MyCircleCheck(float x1, float y1, float r1, float x2, float y2, float r2);

    void CheckCollisions();
    void UpdateBullets();
};

