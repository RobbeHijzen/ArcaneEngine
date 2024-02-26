#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

GameObject::~GameObject() = default;


bool GameObject::AddComponent(std::shared_ptr<BaseComponent> component)
{
	if (component.get())
	{
		m_Components.emplace_back(component);
		return true;
	}
	return false;
}
	

void GameObject::Initialize()
{
	for (auto& component : m_Components)
	{
		component->Initialize();
	}
}

void GameObject::Update()
{
	for (auto& component : m_Components)
	{
		component->Update();
	}
}

void GameObject::FixedUpdate()
{
	for (auto& component : m_Components)
	{
		component->FixedUpdate();
	}
}

void GameObject::LateUpdate()
{
	for (auto& component : m_Components)
	{
		component->LateUpdate();
	}
}

void GameObject::Render() const
{
	for (auto& component : m_Components)
	{
		component->Render();
	}

}

void GameObject::SetParent(GameObject* parent)
{
	// Check for valid parent
	if (IsChild(parent) || parent == this || parent == m_Parent)
	{
		return;
	}



}

void GameObject::SetLocalTransform(float x, float y)
{
	m_LocalTransform.SetPosition(x, y, 0.0f);
}

bool GameObject::IsChild(GameObject* gameObject)
{
	for (auto& child : m_Children)
	{
		if (child == gameObject)
			return true;
	}
	return false;
}

