#pragma once
#include"Field.h"

class Boss1 : public GameObject
{
public:
	Boss1();
	~Boss1();
	void Update()override;
	void Draw() override;
private:
	int hImage;
	float x, y;
};

