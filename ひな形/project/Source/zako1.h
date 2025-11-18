#pragma once
#include"../Library/GameObject.h"
class zako1 : public GameObject
{
public:
	zako1();
	zako1(int sx, int sy);
	~zako1();
	void Update()override;
	void Draw() override;

private:
	int hImage;
	float x, y;
};

