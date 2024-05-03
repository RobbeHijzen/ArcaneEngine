#include "SceneManager.h"
#include "Scene.h"
#include "InputManager.h"

#include <cassert>

namespace AE
{
	void SceneManager::IncrementScene()
	{
		m_Scenes[m_CurrentSceneIndex]->DeleteAll();
		InputManager::GetInstance().RemoveAllBindings();

		m_CurrentSceneIndex = (m_CurrentSceneIndex + 1) % m_Scenes.size();
		m_Scenes[m_CurrentSceneIndex]->Load();

		GameStart();
	}

	void SceneManager::SetScene(int newSceneIndex)
	{
		assert(newSceneIndex < m_Scenes.size() && newSceneIndex >= 0);

		m_Scenes[m_CurrentSceneIndex]->DeleteAll();
		InputManager::GetInstance().RemoveAllBindings();

		m_CurrentSceneIndex = newSceneIndex;
		m_Scenes[m_CurrentSceneIndex]->Load();

		GameStart();
	}

	void SceneManager::GameStart()
	{
		auto scene{ m_Scenes[m_CurrentSceneIndex] };
		if (scene->IsLoaded())
		{
			scene->GameStart();
		}
	}

	void SceneManager::Update()
	{
		auto scene{ m_Scenes[m_CurrentSceneIndex] };
		if (scene->IsLoaded())
		{
			scene->Update();
		}
	}

	void SceneManager::FixedUpdate()
	{
		auto scene{ m_Scenes[m_CurrentSceneIndex] };
		if (scene->IsLoaded())
		{
			scene->FixedUpdate();
		}
	}

	void SceneManager::LateUpdate()
	{
		auto scene{ m_Scenes[m_CurrentSceneIndex] };
		if (scene->IsLoaded())
		{
			scene->LateUpdate();
		}
	}

	void SceneManager::Render()
	{
		auto scene{ m_Scenes[m_CurrentSceneIndex] };
		if (scene->IsLoaded())
		{
			scene->Render();
		}
	}

	void SceneManager::RemoveDeletedObjects()
	{
		auto scene{ m_Scenes[m_CurrentSceneIndex] };
		if (scene->IsLoaded())
		{
			scene->RemoveDeletedObjects();
		}
	}

	Scene& SceneManager::CreateScene(std::unique_ptr<SceneInfo>&& sceneInfo)
	{
		const auto& scene = std::shared_ptr<Scene>(new Scene(std::move(sceneInfo)));
		m_Scenes.emplace_back(scene);

		return *scene;
	}

}
