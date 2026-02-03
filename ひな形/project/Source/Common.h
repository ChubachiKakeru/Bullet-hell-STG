#pragma once
#include "../Library/GameObject.h"

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

    // ★追加：ステージ引き継ぎ用★
    int prevStage;     // どのステージから来たか
    int carryHp;       // 引き継ぎHP
    int carryBomb;     // 引き継ぎBomb
};
