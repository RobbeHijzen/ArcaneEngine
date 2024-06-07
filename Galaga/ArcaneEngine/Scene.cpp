#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

namespace AE
{

	unsigned int Scene::m_idCounter = 0;

	Scene::~Scene() = default;

	void Scene::Load()
	{
		m_SceneInfo->Load(*this);
		m_IsLoaded = true;
	}

	void Scene::Add(std::shared_ptr<GameObject> object)
	{
		m_GameObjects.emplace_back(object);
		if (m_InGame)
		{
			object->GameStart();
		}
	}

	void Scene::DeleteAll()
	{
		m_GameObjects.clear();
	}

	void Scene::RemoveDeletedObjects()
	{
		auto it = m_GameObjects.begin();
		while (it != m_GameObjects.end())
		{
			auto& child = *it;
			if (child->IsDeleted())
			{
				child->NotifyAll(Event::ObjectDestroyed);
				it = m_GameObjects.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void Scene::GameStart()
	{
		m_InGame = true;
		for (int index{}; index < m_GameObjects.size(); ++index)
		{
			auto gameObject{m_GameObjects[index]};
			gameObject->GameStart();
		}
	}

	void Scene::Update()
	{
		for (int index{}; index < m_GameObjects.size(); ++index)
		{
			auto gameObject{ m_GameObjects[index] };
			gameObject->Update();
		}
	}

	void Scene::FixedUpdate()
	{
		for (int index{}; index < m_GameObjects.size(); ++index)
		{
			auto gameObject{ m_GameObjects[index] };
			gameObject->FixedUpdate();
		}
	}

	void Scene::LateUpdate()
	{
		for (int index{}; index < m_GameObjects.size(); ++index)
		{
			auto gameObject{ m_GameObjects[index] };
			gameObject->LateUpdate();
		}
	}

	void Scene::Render() const
	{
		for (int index{}; index < m_GameObjects.size(); ++index)
		{
			auto gameObject{ m_GameObjects[index] };
			if (gameObject->IsVisible())
			{
				gameObject->Render();
			}
		}
	}

	void Scene::AttatchToRoot(std::shared_ptr<GameObject> gameObject)
	{
		m_GameObjects.emplace_back(gameObject);
	}

	void Scene::DettatchFromRoot(GameObject* gameObject)
	{
		m_GameObjects.erase(std::find_if(m_GameObjects.begin(), m_GameObjects.end(), [&](const std::shared_ptr<GameObject>& sp)
			{
				return sp.get() == gameObject;
			}));
	}

	std::shared_ptr<GameObject> Scene::GetChildSharedPtr(GameObject* child)
	{
		for (auto& currentChild : m_GameObjects)
		{
			if (currentChild.get() == child)
			{
				return currentChild;
			}
		}
		return nullptr;
	}

}