#include "Bird.h"
#include"Field.h"

Bird::Bird()
{
	hImage = LoadGraph("data/image/bird.png");
	x = 300;
	y = 100;
}

Bird::Bird(int sx, int sy)
{
	hImage = LoadGraph("data/image/bird.png");
	x = sx;
	y = sy;
}

Bird::~Bird()
{
}

void Bird::Update()
{
}

void Bird::Draw()
{
	Field* field = FindGameObject<Field>();
	DrawRectGraph(x, y, 0, 0, 64, 64, hImage, 1);
}