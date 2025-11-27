#pragma once
#include"Field.h"
#include"../Library/GameObject.h"

class Player;
class Bulett;

enum class Zako1Pattern {
	CIRCLE,       // 円軌道
	FIGURE_EIGHT, // 八の字
	LEFT_RIGHT    // 左右移動
};

class zako1 : public GameObject
{
public:
	zako1();
	zako1(int sx, int sy);
	~zako1();
	void Update()override;
	void Draw() override;

	bool IsActive() const { return isActive; }
	int GetHP() const { return hp; }
	void TakeDamage(int damage);
	bool IsHit(float bx, float by, int rad);

	float GetX() const { return x; }
	float GetY() const { return y; }

	bool IsAlive() const { return isActive; }
	float GetCenterX() const { return x; }
	float GetCenterY() const { return y; }
	float GetSize() const { return size; }

private:
	int hImage;
	float x, y;
	float centerX, centerY;
	int hp;
	bool isActive;
	float size;

	// 判定サイズ
	Zako1Pattern pattern;
	float rectWidth;
	float rectHeight;
	float circleRadius;

	// 移動パターン関連
	float moveTimer;
	float patternChangeTime;
	float patternTimer;

	// 左右移動用
	float speed;
	int direction;

	void UpdateLeftRight();
};

