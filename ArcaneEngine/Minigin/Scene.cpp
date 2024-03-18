#include "Scene.h"
#include "GameObject.h"

#include <algorithm>


unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) 
{
}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_GameObjects.emplace_back(object);
}

void Scene::RemoveAll()
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
	for (auto& gameObject : m_GameObjects)
	{
		gameObject->GameStart();
	}
}

void Scene::Update()
{
	for (auto& gameObject : m_GameObjects)
	{
		gameObject->Update();
	}
}

void Scene::FixedUpdate()
{
	for (auto& gameObject : m_GameObjects)
	{
		gameObject->FixedUpdate();
	}
}

void Scene::LateUpdate()
{
	for (auto& gameObject : m_GameObjects)
	{
		gameObject->LateUpdate();
	}
}

void Scene::Render() const
{
	for (auto& gameObject : m_GameObjects)
	{
		gameObject->Render();
	}
}

void Scene::AttatchToRoot(std::shared_ptr<GameObject> gameObject)
{
	m_GameObjects.emplace_back(gameObject);
}

void Scene::DettatchFromRoot(GameObject* gameObject)
{
	auto it = m_GameObjects.begin();
	while (it != m_GameObjects.end())
	{
		auto& child = *it;
		if (child.get() == gameObject)
		{
			it = m_GameObjects.erase(it);
			return;
		}
		++it;
	}
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

