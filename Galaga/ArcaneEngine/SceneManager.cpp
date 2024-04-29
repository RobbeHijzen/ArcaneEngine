#include "SceneManager.h"
#include "Scene.h"


namespace AE
{
	void SceneManager::GameStart()
	{
		m_Scenes[m_CurrentSceneIndex]->GameStart();
	}

	void SceneManager::Update()
	{
		m_Scenes[m_CurrentSceneIndex]->Update();
	}

	void SceneManager::FixedUpdate()
	{
		m_Scenes[m_CurrentSceneIndex]->FixedUpdate();
	}

	void SceneManager::LateUpdate()
	{
		m_Scenes[m_CurrentSceneIndex]->LateUpdate();
	}

	void SceneManager::Render()
	{
		m_Scenes[m_CurrentSceneIndex]->Render();
	}

	Scene& SceneManager::CreateScene(const std::string& name)
	{
		const auto& scene = std::shared_ptr<Scene>(new Scene(name));
		m_Scenes.push_back(scene);
		++m_CurrentSceneIndex;

		return *scene;
	}

	void SceneManager::RemoveDeletedObjects()
	{
		m_Scenes[m_CurrentSceneIndex]->RemoveDeletedObjects();
	}
}
