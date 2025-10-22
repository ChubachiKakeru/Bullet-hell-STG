#include "Boss1.h"
#include"Field.h"

Boss1::Boss1()
{
	hImage = LoadGraph("data/image/bird.png");
	

	
}

Boss1::~Boss1()
{
}

void Boss1::Update()
{

}

void Boss1::Draw()
{
	DrawGraph(200, 100, hImage, TRUE);
}

//int player, enemy;
//int px = 200;
//int ex = 0, es = 8;
//int WINAPI WinMain2(HINSTANCE, LPSTR, int)
//{
//	ChangeWindowMode(TRUE);
//	SetDrawScreen(DX_SCREEN_BACK);
//	player = LoadGraph("data/image/luffy.png");
//	enemy = LoadGraph("data/image/enel.png");
//
//	ex += es;
//	if (ex <= 0 || ex >= 640 - 64)es = -es;
//	return 0;
//}
