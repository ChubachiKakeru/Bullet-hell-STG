#include "Player.h"
#include "Field.h"
#include "playerBullet.h"
#include "Common.h"
#include"Screen.h"

// ========================================
// 定数定義
// ========================================
namespace {
    constexpr float MOVE_SPEED = 7.0f;
    constexpr float SHOT_COOLDOWN = 50.0f;
    constexpr int INITIAL_HP = 100;
    constexpr float PLAYER_COLLISION_RADIUS = 30.0f;
    constexpr float PLAYER_CENTER_OFFSET = 60.0f;
    constexpr float BULLET_SPEED = -10.0f;
    constexpr float BULLET_RADIUS = 8.0f;
}

// ========================================
// コンストラクタ / デストラクタ
// ========================================
Player::Player() : GameObject()
{
    hImage = LoadGraph("data/image/file/chara/player.png");
    x = 300;
    y = 900;
    velocity = 0;
    onGround = false;
    shotTimer = 0.0f;
    hp = INITIAL_HP;
    isActive = true;
}

Player::Player(int sx, int sy) : GameObject()
{
    hImage = LoadGraph("data/image/file/chara/player.png");
    x = sx;
    y = sy;
    velocity = 0;
    onGround = false;
    shotTimer = 0.0f;
    hp = INITIAL_HP;
    isActive = true;
}

Player::~Player()
{
    if (hImage != -1) {
        DeleteGraph(hImage);
    }
}

// ========================================
// ダメージ処理
// ========================================
void Player::TakeDamage(int damage)
{
    hp -= damage;
    if (hp <= 0) {
        hp = 0;
        SceneManager::ChangeScene("TITLE");
    }
}

// ========================================
// 当たり判定
// ========================================
bool Player::IsHit(float bx, float by, int rad)
{
    float dx = bx - (x + PLAYER_CENTER_OFFSET);
    float dy = by - (y + PLAYER_CENTER_OFFSET);
    float distance = sqrt(dx * dx + dy * dy);

    if (distance < PLAYER_COLLISION_RADIUS + rad) {
        TakeDamage(0);
        return true;
    }
    return false;
}

// ========================================
// 更新処理
// ========================================
void Player::Update()
{
    Field* field = FindGameObject<Field>();
    if (!field) return;

    // 移動前の座標を保存
    float prevX = x;
    float prevY = y;

    // 右移動 (Dキー)
    if (CheckHitKey(KEY_INPUT_D)) {
        x += MOVE_SPEED;
        int push1 = field->HitCheckRight(x + 50/2, y + 5);
        int push2 = field->HitCheckRight(x + 50/2, y + 63);
        x -= max(push1, push2);
    }

    // 左移動 (Aキー)
    if (CheckHitKey(KEY_INPUT_A)) {
        x -= MOVE_SPEED;
        int push1 = field->HitCheckLeft(x + 14, y + 5);
        int push2 = field->HitCheckLeft(x + 14, y + 63);
        x -= max(push1, push2);
    }

    // 上移動 (Wキー)
    if (CheckHitKey(KEY_INPUT_W)) {
        y -= MOVE_SPEED;
        int push1 = field->HitCheckUp(x + 14, y + 5);
        int push2 = field->HitCheckUp(x + 50/2, y + 5);
        int push = max(push1, push2);
        if (push > 0) {
            y += push;
            velocity = 0;
        }
    }

    // 下移動 (Sキー)
    if (CheckHitKey(KEY_INPUT_S)) {
        y += MOVE_SPEED;
        int push1 = field->HitCheckDown(x + 14, y + 64);
        int push2 = field->HitCheckDown(x + 50, y + 64);
        int push = max(push1, push2);
        if (push > 0) {
            y -= push - 1;
            velocity = 0;
            onGround = false;
        }
    }

    // ステージ境界内に制限（画面の青い部分のみ）
    if (x < Field::STAGE_LEFT) x = Field::STAGE_LEFT;
    if (x > Field::STAGE_RIGHT - 50/2) x = Field::STAGE_RIGHT - 50/2;  // プレイヤー幅を考慮
    if (y < Field::STAGE_TOP) y = Field::STAGE_TOP;
    if (y > Screen::HEIGHT - 100) y = Screen::HEIGHT - 100;  // 画面下端まで移動可能

    // 弾発射タイマー更新
    shotTimer += 1.0f;

    // Hキーで弾発射
    if (CheckHitKey(KEY_INPUT_H) && shotTimer >= SHOT_COOLDOWN) {
        ShootBullet();
        shotTimer = 0.0f;
    }
}


// ========================================
// 弾発射処理
// ========================================
void Player::ShootBullet()
{
    // プレイヤーの中心から弾を発射
    int bulletX = (int)x + 50 / 2;
    int bulletY = (int)y + 50 / 2;
    new playerBullet(bulletX, bulletY, 0, BULLET_SPEED, BULLET_RADIUS);
}

// ========================================
// 描画処理
// ========================================
void Player::Draw()
{

    DrawGraph(x, y, hImage, TRUE);
    Field* field = FindGameObject<Field>();
}


