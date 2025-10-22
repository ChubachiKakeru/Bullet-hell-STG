#include "Boss1.h"
#include"Field.h"

Boss1::Boss1() : active(true)
{
	hImage = LoadGraph("data/image/bird.png");
	x = 0;
	y = 16;
	speed = 6;
	

	
}

Boss1::~Boss1()
{
}

void Boss1::Update()
{
	if (!active)
		return;
	x += speed;
	if (x <= 0 || x >= 640 - 64) {
		speed = -speed;
	}
	/*if (!active)
		return;
	y += speed;
	if (y< 0 || x >= 640 - 64) {
		speed = -speed;
	}*/
}

void Boss1::Draw()
{
	if (active && hImage != -1) {
		DrawGraph(x, y, hImage, TRUE);
	}
}

void Boss1::Reset()
{
	x = 0;
	y = 16;
	speed = 8;
	active = true;
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
