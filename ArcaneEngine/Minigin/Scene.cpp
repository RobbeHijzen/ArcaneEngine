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
	object->SetParent(m_Root.get());
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_Root->RemoveChild(object.get());
}

void Scene::RemoveAll()
{
	m_Root->RemoveAllChildren();
}

void Scene::Initialize()
{
	m_Root->Initialize();
}

void Scene::Update()
{
	m_Root->Update();
}

void Scene::FixedUpdate()
{
	m_Root->FixedUpdate();
}

void Scene::LateUpdate()
{
	m_Root->LateUpdate();
}

void Scene::Render() const
{
	m_Root->Render();
}

