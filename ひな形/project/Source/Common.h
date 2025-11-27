#pragma once
#include"../Library/GameObject.h"

class Common : public GameObject
{
public:
	Common();
	~Common();
	void Draw() override;

	int stageNumber;
	int score;
	int hiScore;
	bool noDead;
};
