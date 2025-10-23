#pragma once
#include"../Library/GameObject.h"
#include"define.h"
#include "Shot.h"

class Player : public GameObject
{
public:
	Player();
	Player(int sx, int sy);
	~Player();
	void Update()override;
	void Draw() override;
	void All();


	

private:
	int hImage;
	int time;

	Shot* shot; // Å© É|ÉCÉìÉ^Ç≈ä«óù
	double x, y;
	int speed;
	int width, height;
	float move;
	int xcount, ycount;
	int ix, iy, result;
	bool life;
	int count;

};

