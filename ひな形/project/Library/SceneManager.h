#pragma once
#include <string>
#include <list>

class SceneFactory;
class SceneBase;

namespace SceneManager
{
	void Init(); // ‰Šú‰»
	void Update(); // XV
	void Draw(); // •`‰æ
	void Release(); // ŠJ•ú

	void ChangeScene(const std::string& sceneName);

	SceneBase* CurrentScene();

	void Exit();

	bool IsExit();
};

template<class C>
C* GetScene() {
	return dynamic_cast<C*>(SceneManager::CurrentScene());
}