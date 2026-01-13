#include "TitleScene.h"
#include <cassert>

TitleScene::TitleScene()
{
	//画像サイズを取得
	int screenW, screenH;
	GetDrawScreenSize(&screenW, &screenH);

	hImage = LoadGraph("data/image/弾幕.png");
	assert(hImage, &orgW, &orgH);


	//元の大きさを取得
	GetGraphSize(hImage, &orgW, &orgH);


	font = CreateFontToHandle("はるかぜ B AP", 30, 6);
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
	if (CheckHitKey(KEY_INPUT_P)) {
		SceneManager::ChangeScene("PLAY");
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE)) {
		SceneManager::Exit();
	}
}


void TitleScene::Draw()
{
	int screenW, screenH;
	GetDrawScreenSize(&screenW, &screenH);

	// 中央座標
	int x = (screenW - orgW) / 2;
	int y = (screenH - orgH) / 2;

	// 元サイズのまま描画
	DrawGraph(x, y, hImage, TRUE);

	extern const char* Version();
	DrawString(0, 20, Version(), GetColor(255,255,255));
	DrawString(0, 0, "TITLE SCENE", GetColor(255,255,255));
	//DrawFormatString(100, 100, GetColor(255,255,255), "%4.1f", 1.0f / Time::DeltaTime());
	DrawString(520, 1000, "Pキーを押してプレイ", GetColor(255, 255, 255));
}
