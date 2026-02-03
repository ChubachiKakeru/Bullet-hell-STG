#pragma once

#include "myDxLib.h"
#include <string>
#include "ObjectManager.h"
#include "SceneManager.h"
#include "Time.h"

class GameObject
{
public:
	GameObject() : tag(""), destroy(false), dontDestroy(false), drawOrder(0) { ObjectManager::Push(this); }
	virtual ~GameObject() { ObjectManager::Pop(this); }
	virtual void Update() {}
	virtual void Draw() {}

	void DestroyMe() { destroy = true; }

	inline bool DestroyRequested() const { return destroy; }
	void DontDestroyOnSceneChange(bool sw = true) { dontDestroy = sw; }

	bool IsDontDestroy() const { return dontDestroy; }
	void SetDrawOrder(int order)
	{
		drawOrder = order;
		ObjectManager::SortByDrawOrder();
	}

	inline int GetDrawOrder() const { return drawOrder; }
	void SetTag(std::string _tag) { tag = _tag; }

	bool IsTag(std::string _tag) const { return tag == _tag; }
private:
	std::string tag;     // タグ
	bool destroy;
	bool dontDestroy;
	int drawOrder;
	int hImage;
	float x, y;
};

// 旧バージョンとの互換性のため残しています。
// 削除予定のため、使わないでください。
template <class C>
C* Instantiate()
{
	return new C();
}