#pragma once
#include"../Library/GameObject.h"
class Shot:public GameObject
{
public:
	Shot();
	~Shot();
	void Update() override;
	void Draw() override;
	void Fire(int sx, int sy);

	int GetX() const { return (int)x; }
	int GetY() const { return (int)y; }
	bool IsActive() const { return active; }
	void SetActive(bool state) { active = state; }

	void SetPlayerPos(int px,int py) { playerX = px; playerY = py; }

private:
	int speed;
	bool active;
	bool sf;
	float x, y;
	int hImage;
	int playerX;
	int playerY;
	int sx, sy;
};

