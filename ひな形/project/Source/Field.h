#pragma once
#include"../Library/GameObject.h"



class Field : public GameObject
{
public:
	Field();
	~Field();
	void Update() override;
	void Draw() override;

	int HitCheckRight(int px, int py);
	int HitCheckLeft(int px, int py);
	int HitCheckDown(int px, int py);
	int HitCheckUp(int px, int py);

	// スクロール取得
	int GetScrollY() const { return scrollY; }
	float GetScrollSpeed() const { return scrollSpeed; }

private:
	int hImage;
	float x, y;
	int scrollX;
	int scrollY;
	float scrollSpeed;     // スクロール速度

};
