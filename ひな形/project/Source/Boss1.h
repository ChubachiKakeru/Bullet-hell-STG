#pragma once
#include"Field.h"

class Boss1 : public GameObject
{
public:
	Boss1();
	~Boss1();
	void Update()override;
	void Draw() override;
	void Reset();

	int GetX() const {return(int)x;}
	int GetY() const { return(int)y; }
	bool IsActive() const { return active; }

private:
	int hImage;
	float x, y;
	int speed;
	bool active;
};

