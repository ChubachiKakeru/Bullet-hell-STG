#include "Collision.h"
#include "Player.h"
#include "Boss1.h"
#include "Bullet.h"
#include <cmath>

Collision* Collision::instance = nullptr;

Collision::Collision() : GameObject() {
    instance = this;
}

Collision::~Collision() {
    for (auto b : playerBullets) delete b;
    for (auto b : bossBullets) delete b;
    playerBullets.clear();
    bossBullets.clear();
    instance = nullptr;
}

void Collision::Update() {
    UpdateBullets();
    CheckCollisions();
}

void Collision::UpdateBullets() {
    // プレイヤー弾更新
    for (auto it = playerBullets.begin(); it != playerBullets.end();) {
        (*it)->Update();
        if (!(*it)->IsAlive()) {
            delete* it;
            it = playerBullets.erase(it);
        }
        else {
            ++it;
        }
    }

    // ボス弾更新
    for (auto it = bossBullets.begin(); it != bossBullets.end();) {
        (*it)->Update();
        if (!(*it)->IsAlive()) {
            delete* it;
            it = bossBullets.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Collision::CheckCollisions() {
    Player* player = FindGameObject<Player>();
    if (player == nullptr || !player->IsAlive()) return;

    Boss1* boss = FindGameObject<Boss1>();

    // プレイヤー弾 vs ボス
    if (boss != nullptr && boss->IsAlive()) {
        for (auto it = playerBullets.begin(); it != playerBullets.end();) {
            Bullet* b = *it;

            bool hit = MyCircleCheck(
                b->GetCenterX(), b->GetCenterY(), b->GetSize(),
                boss->GetCenterX(), boss->GetCenterY(), boss->GetSize()
            );

            if (hit) {
                boss->Hit(10);
                delete b;
                it = playerBullets.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    // ボス弾 vs プレイヤー
    for (auto it = bossBullets.begin(); it != bossBullets.end();) {
        Bullet* b = *it;

        bool hit = MyCircleCheck(
            b->GetCenterX(), b->GetCenterY(), b->GetSize(),
            player->GetCenterX(), player->GetCenterY(), player->GetSize()
        );

        if (hit) {
            player->Hit(10);
            delete b;
            it = bossBullets.erase(it);
        }
        else {
            ++it;
        }
    }
}

bool Collision::MyCircleCheck(float x1, float y1, float r1, float x2, float y2, float r2) {
    // ★★★ ここに円形当たり判定を実装 ★★★
    // 【第一段階】矩形による粗い判定
    float left1 = x1 - r1;
    float right1 = x1 + r1;
    float top1 = y1 - r1;
    float bottom1 = y1 + r1;

    float left2 = x2 - r2;
    float right2 = x2 + r2;
    float top2 = y2 - r2;
    float bottom2 = y2 + r2;

    if (right1 < left2 || left1 > right2 || bottom1 < top2 || top1 > bottom2) {
        return false;
    }

    // 【第二段階】円による正確な判定（平方根を使わない）
    float dx = x2 - x1;
    float dy = y2 - y1;
    float distanceSquared = dx * dx + dy * dy;  // 距離の2乗
    float radiusSum = r1 + r2;
    float radiusSumSquared = radiusSum * radiusSum;  // 半径の和の2乗

    // 距離の2乗と半径の和の2乗を比較
    return distanceSquared < radiusSumSquared;
}
