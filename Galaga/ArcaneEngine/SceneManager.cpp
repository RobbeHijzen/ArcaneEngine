#include "SceneManager.h"
#include "Scene.h"

#include <cassert>

namespace AE
{
	void SceneManager::IncrementScene()
	{
		m_Scenes[m_CurrentSceneIndex]->DeleteAll();
		m_CurrentSceneIndex = (m_CurrentSceneIndex + 1) % m_Scenes.size();

		GameStart();
	}

	void SceneManager::SetScene(int newSceneIndex)
	{
		assert(newSceneIndex < m_Scenes.size() && newSceneIndex >= 0);

		m_Scenes[m_CurrentSceneIndex]->DeleteAll();
		m_CurrentSceneIndex = newSceneIndex;

		GameStart();
	}

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

	void SceneManager::RemoveDeletedObjects()
	{
		m_Scenes[m_CurrentSceneIndex]->RemoveDeletedObjects();
	}

	Scene& SceneManager::CreateScene()
	{
		const auto& scene = std::shared_ptr<Scene>(new Scene());
		m_Scenes.emplace_back(scene);

		return *scene;
	}

}
