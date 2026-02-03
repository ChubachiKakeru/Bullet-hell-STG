#include "Common.h"
#include "../ImGui/imgui.h"

Common::Common()
{
    DontDestroyOnSceneChange();

    stageNumber = 1;
    score = 0;
    hiScore = 0;
    noDead = false;

    // ★追加★
    prevStage = -1;
    carryHp = 0;
    carryBomb = 0;
}

Common::~Common()
{
}

void Common::Draw()
{
    // デバッグ表示用（任意）
}
