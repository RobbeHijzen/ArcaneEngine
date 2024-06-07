#include "SceneManager.h"
#include "Scene.h"
#include "InputManager.h"
#include "TimeManager.h"

#include <cassert>

namespace AE
{
	void SceneManager::SetScene(std::string sceneName)
	{
		for (int index{}; index < m_Scenes.size(); ++index)
		{
			auto scene{m_Scenes[index]};
			if (scene->GetSceneName() == sceneName)
			{
				m_Scenes[m_CurrentSceneIndex]->DeleteAll();
				InputManager::GetInstance().RemoveAllBindings();
				TimeManager::GetInstance().ClearAllTimers();

				m_CurrentSceneIndex = index;
				m_Scenes[m_CurrentSceneIndex]->Load();


				GameStart();
				break;
			}
		}
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

	Scene& SceneManager::CreateScene(std::unique_ptr<SceneInfo>&& sceneInfo, std::string sceneName)
	{
		const auto& scene = std::shared_ptr<Scene>(new Scene(std::move(sceneInfo), sceneName));
		m_Scenes.emplace_back(scene);

		return *scene;
	}

}
