#pragma once
#include"Field.h"
#include "../Library/GameObject.h"

class Player;
class Bulett;

enum class BossPattern {
	CIRCLE,       // 円軌道
	FIGURE_EIGHT, // 八の字
	LEFT_RIGHT    // 左右移動
};

class Boss1 : public GameObject
{
public:
	Boss1();
	Boss1(int sx, int sy);
	~Boss1();
	void Update()override;
	void Draw() override;

	bool IsActive() const { return isActive; }
	int GetHP() const { return hp; }
	void TakeDamage(int damage);
    void Hit();

    float GetX() const { return x; }
    float GetY() const { return y; }

    bool IsAlive() const { return isAlive; }
    float GetCenterX() const { return x; }
    float GetCenterY() const { return y; }

private:
	int hImage;
	float x, y;
    float centerX, centerY;
    int hp;
    bool isActive;
    bool isAlive;

    // 判定サイズ
    float rectWidth;
    float rectHeight;
    float circleRadius;

    // 移動パターン関連
    BossPattern pattern;
    float moveTimer;
    float patternChangeTime;
    float patternTimer;

    // 円軌道用
    float radius;
    float angle;
    float angularSpeed;

    // 八の字用
    float figureEightScale;

    // 左右移動用
    float speed;
    int direction;

    // 弾発射関連
    float shotTimer;
    float shotInterval;

    void UpdateCircle();
    void UpdateFigureEight();
    void UpdateLeftRight();
    void ChangePattern();
    void ShootBullet();
};

