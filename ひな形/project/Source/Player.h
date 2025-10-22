#pragma once
#include"../Library/GameObject.h"

class Player : public GameObject
{
public:
	Player();
	Player(int sx, int sy);
	~Player();
	void Update()override;
	void Draw() override;

	int GetX()const { return(int)x; }
	int GetY()const { return(int)y; }
	int GetShotX()const { return(int)x + 24; }
	int GetShotY()const { return(int)y; }

private:
	int hImage;
	float x, y;
	int speed;

};

