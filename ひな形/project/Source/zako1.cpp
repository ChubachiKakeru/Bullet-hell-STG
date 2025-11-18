#include "zako1.h"

zako1::zako1()
{
	hImage = LoadGraph("data/image/zako1.png");
	x = 100;
	y = 100;
}

zako1::zako1(int sx, int sy)
{
	hImage = LoadGraph("data/image/zako1.png");
	x = sx;
	y = sy;
}

zako1::~zako1()
{
}

void zako1::Update()
{
}

void zako1::Draw()
{
	DrawGraph(x, y, hImage, TRUE);
}

