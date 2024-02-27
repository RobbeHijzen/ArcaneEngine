#include "Scene.h"
#include "GameObject.h"

#include <algorithm>


unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) 
{
	m_Root = std::make_shared<GameObject>();
}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	//object->SetParent(m_Root.get());
	m_GameObjects.emplace_back(object);
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	//m_Root->RemoveChild(object.get());
	m_GameObjects.erase(std::remove(m_GameObjects.begin(), m_GameObjects.end(), object), m_GameObjects.end());
}

void Scene::RemoveAll()
{
	//m_Root->RemoveAllChildren();v
	m_GameObjects.clear();
}

void Scene::RemoveDeletedObjects()
{
	auto children = m_Root->GetChildren();
	auto it = children.begin();
	while (it != children.end()) 
	{
		auto& child = *it;
		if (child->IsDeleted()) 
		{
			it = children.erase(it);
		}
		else 
		{
			++it; 
		}
	}
}

void Scene::Initialize()
{
	//m_Root->Initialize();
	for (auto& gameObject : m_GameObjects)
	{
		gameObject->Initialize();
	}
}

void Scene::Update()
{
	//m_Root->Update();
	for (auto& gameObject : m_GameObjects)
	{
		gameObject->Update();
	}
}

void Scene::FixedUpdate()
{
	//m_Root->FixedUpdate();
	for (auto& gameObject : m_GameObjects)
	{
		gameObject->FixedUpdate();
	}
}

void Scene::LateUpdate()
{
	//m_Root->LateUpdate();
	for (auto& gameObject : m_GameObjects)
	{
		gameObject->LateUpdate();
	}
}

void Scene::Render() const
{
	//m_Root->Render();
	for (auto& gameObject : m_GameObjects)
	{
		gameObject->Render();
	}
}

