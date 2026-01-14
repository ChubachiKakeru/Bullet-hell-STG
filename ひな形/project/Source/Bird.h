#pragma once
#include"../Library/GameObject.h"
class Bird : public GameObject
{
public:
	Bird();
	Bird(int sx, int sy);
	~Bird();
	void Update() override;
	void Draw() override;

private:
	int x, y;
	int hImage;
};

