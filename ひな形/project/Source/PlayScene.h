#pragma once
#include "../Library/SceneBase.h"
#include "Player.h"
#include "Boss1.h"
#include "Boss2.h"
#include "Bullet.h"
#include <vector>

/// <summary>
/// ゲームプレイのシーンを制御する
/// </summary>
class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;
    // 弾の発射
    void FirePlayerBullet();
    void FireBoss1Bullet();

private:
    Player player;
    Boss1 boss;
    std::vector<Bullet*> playerBullets;
    std::vector<Bullet*> bossBullets;

    // 当たり判定用の関数
    void CheckCollisions();
    void RemoveDeadBullets(std::vector<Bullet*>& bullets);  // 死んだ弾削除用

    Boss1* boss1;
    Boss2* boss2;

    bool boss1Dead;          // Boss1が倒されたか
    bool boss2Spawned;       // Boss2を出したか
    int boss2SpawnTimer;     // 出現待ちタイマー

};
