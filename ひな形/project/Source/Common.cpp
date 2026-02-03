#include "Common.h"
#include"../ImGui/imgui.h"

Common::Common()
{
	DontDestroyOnSceneChange();//Sceane‚ª•Ï‚í‚Á‚Ä‚àÁ‚¦‚È‚¢
	stageNumber = 1;
	bool noDead = false;
}

Common::~Common()
{
}

void Common::Draw()
{
}
