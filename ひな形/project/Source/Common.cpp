#include "Common.h"
#include"../ImGui/imgui.h"

Common::Common()
{
	DontDestroyOnSceneChange();//Sceane‚ª•Ï‚í‚Á‚Ä‚àÁ‚¦‚È‚¢
	stageNumber = 1;
}

Common::~Common()
{
}

void Common::Draw()
{
	//ImGui::Begin("Common");
	//ImGui::InputInt("StageNumber", &stageNumber);
	//ImGui::Checkbox("noDead", &noDead);
	//ImGui::End();
}
