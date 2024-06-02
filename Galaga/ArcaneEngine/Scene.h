#pragma once
#include "SceneManager.h"
#include "SceneInfo.h"

namespace AE
{
	class GameObject;

	class Scene final
	{
		friend Scene& SceneManager::CreateScene(std::unique_ptr<SceneInfo>&& sceneInfo, std::string sceneName);
	public:

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;


		void Load();
		bool IsLoaded() const { return m_IsLoaded; }

		void Add(std::shared_ptr<GameObject> object);
		void DeleteAll();


		void RemoveDeletedObjects();

		void GameStart();

		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;

		void AttatchToRoot(std::shared_ptr<GameObject> gameObject);
		void DettatchFromRoot(GameObject* gameObject);
		std::shared_ptr<GameObject> GetChildSharedPtr(GameObject* child);

		auto GetGameObjects() const { return m_GameObjects; }
		std::string GetSceneName() const { return m_Name; }

	private:

		explicit Scene(std::unique_ptr<SceneInfo>&& sceneInfo, std::string sceneName) 
			: m_SceneInfo{ std::move(sceneInfo) } 
			, m_Name{sceneName}
		{}

		std::string m_Name;
		std::vector<std::shared_ptr<GameObject>> m_GameObjects{};

		std::unique_ptr<SceneInfo> m_SceneInfo;
		bool m_IsLoaded{ false };

		static unsigned int m_idCounter;
	};
}


