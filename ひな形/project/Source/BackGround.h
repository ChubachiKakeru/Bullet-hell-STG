#pragma once
#include "../Library/GameObject.h"

class BackGround : public GameObject
{
public:
    BackGround();
    BackGround(float offX, float offY);
    ~BackGround();

    int HitCheckLeft(int px, int py);

    void Update() override;
    void Draw() override;

    // 位置調整用
    void SetOffset(float x, float y) { offsetX = x; offsetY = y; }
    void SetScale(float s) { scale = s; }
	float GetscrollY()const { return scrollY; }

private:
    int hImage;
    int hImage2;
    int scrollY;
	float totalscrollY;
    float scrollSpeed;
    float offsetX;      // X方向のオフセット
    float offsetY;      // Y方向のオフセット
    float scale;        // 拡大率
};

