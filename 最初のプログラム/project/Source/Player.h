#pragma once
#include "../Library/GameObject.h"

// プレイヤーのことをやる処理を書くところ
class Player : public GameObject
{
public:
	Player(); // コンストラクター（最初に１回呼ばれる）
	~Player(); // デストラクター（最後に１回呼ばれる）
	void Update() override; // 毎フレーム呼ばれる　計算
	void Draw() override; // 毎フレーム呼ばれる　描画

	// メンバー変数（プレイヤーを処理するのに必要な変数
	int image;
	float posX;
	float posY;
};