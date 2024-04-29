#pragma once
#include "SceneManager.h"

namespace AE
{
	class GameObject;

	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;


		void Add(std::shared_ptr<GameObject> object);
		void RemoveAll();


		void RemoveDeletedObjects();

		void GameStart();

		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;

		void AttatchToRoot(std::shared_ptr<GameObject> gameObject);
		void DettatchFromRoot(GameObject* gameObject);
		std::shared_ptr<GameObject> GetChildSharedPtr(GameObject* child);

	private:
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector<std::shared_ptr<GameObject>> m_GameObjects{};

		static unsigned int m_idCounter;
	};
}


