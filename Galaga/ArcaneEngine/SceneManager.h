#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace AE
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);


		Scene* GetCurrentScene() { return m_Scenes[m_CurrentSceneIndex].get(); }

		void RemoveDeletedObjects();

		void GameStart();

		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render();
	private:

		friend class Singleton<SceneManager>;
		SceneManager() = default;

		std::vector<std::shared_ptr<Scene>> m_Scenes;
		int m_CurrentSceneIndex{ -1 };
	};
}
