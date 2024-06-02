#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include "SceneInfo.h"

namespace AE
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(std::unique_ptr<SceneInfo>&& sceneInfo, std::string sceneName);
		Scene* GetCurrentScene() { return m_Scenes[m_CurrentSceneIndex].get(); }

		int GetCurrentSceneIndex() const { return m_CurrentSceneIndex; }
		void SetScene(std::string sceneName);

		void GameStart();

		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render();

		void RemoveDeletedObjects();

	private:

		friend class Singleton<SceneManager>;
		SceneManager() = default;

		std::vector<std::shared_ptr<Scene>> m_Scenes;
		int m_CurrentSceneIndex{ 0 };
	};
}
