#pragma once
#include"../Library/GameObject.h"



class Field : public GameObject
{
public:
    // ステージ境界の定義
    static const int STAGE_LEFT = 20;     // ステージ左端（画面の青い部分の左側）
    static const int STAGE_RIGHT = 714;   // ステージ右端（画面の青い部分の右側）
    static const int STAGE_TOP = 80;      // ステージ上端
    static const int STAGE_BOTTOM = 1280;  // ステージ下端

    // 座標がステージ内かチェック
    static bool IsInStage(float x, float y) {
        return x >= STAGE_LEFT && x <= STAGE_RIGHT &&
            y >= STAGE_TOP && y <= STAGE_BOTTOM;
    }

    // 座標をステージ内に制限
    static void ClampToStage(float& x, float& y) {
        if (x < STAGE_LEFT) x = (float)STAGE_LEFT;
        if (x > STAGE_RIGHT) x = (float)STAGE_RIGHT;
        if (y < STAGE_TOP) y = (float)STAGE_TOP;
        if (y > STAGE_BOTTOM) y = (float)STAGE_BOTTOM;
    }

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
