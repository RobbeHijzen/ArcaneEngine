#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include "SceneInfo.h"
#include "GameInstance.h"

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

		void SetGameInstance(std::unique_ptr<GameInstance> gameInstance) { m_GameInstance = std::move(gameInstance); }
		GameInstance* GetGameInstance() const { return m_GameInstance.get(); }

	private:

		friend class Singleton<SceneManager>;
		SceneManager() = default;

		std::vector<std::shared_ptr<Scene>> m_Scenes;
		int m_CurrentSceneIndex{ 0 };

		std::unique_ptr<GameInstance> m_GameInstance{};
	};
}
