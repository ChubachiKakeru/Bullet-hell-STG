#pragma once
#include "../Library/SceneBase.h"
#include "Player.h"
#include "Boss1.h"
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

private:
    Player player;
    Boss1 boss;
    std::vector<Bullet> playerBullets;
    std::vector<Bullet> bossBullets;

    // 当たり判定用の関数
    void CheckCollisions();
    void RemoveDeadBullets(std::vector<Bullet*>& bullets);  // 死んだ弾削除用


};
