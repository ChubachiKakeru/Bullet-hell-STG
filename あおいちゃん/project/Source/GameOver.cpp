#include "GameOver.h"
#include <assert.h>

GameOver::GameOver()
{
	hImage = LoadGraph("data/image/gameover.png");
	assert(hImage>0);
}

GameOver::~GameOver()
{
}

void GameOver::Update()
{
}

void GameOver::Draw()
{
	DrawGraph(200, 100, hImage, TRUE);
}
