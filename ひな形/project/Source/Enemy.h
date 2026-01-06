#pragma once
#include"../Library/GameObject.h"

class Enemy : public GameObject {
public:
	Enemy();
	Enemy(float sx, float sy, float vx, float vy, int health);
	~Enemy();
	void Update() override;
	void Draw() override;
	void TakeDamage(int dmg);
	bool IsDead() const { return isDead || hp <= 0; }
	bool IsOutOfScreen() const;

	float GetX() const { return x; }
	float GetY() const { return y; }
	int GetHP() const { return hp; }

private:
	float x, y;
	float vx, vy;
	int hp;
	int isDead;
	int hImage;
};
