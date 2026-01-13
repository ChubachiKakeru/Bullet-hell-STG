#pragma once

class DebugSceneSelector
{
public:
    static void Update();
    static void Draw();

private:
    static bool m_isActive;
    static int m_selectedScene;
    static const int SCENE_COUNT = 6;
};