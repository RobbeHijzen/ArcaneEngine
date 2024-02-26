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

template<typename T>
inline std::shared_ptr<T> GameObject::GetComponent()
{
	for (auto& component : m_Components)
	{
		if (auto castedComponent = std::dynamic_pointer_cast<T>(component))
		{
			return castedComponent;
		}
	}
	return nullptr;
}
	
template<typename T>
void GameObject::RemoveComponent()
{
	for (auto it = m_Components.begin(); it != m_Components.end(); ++it) 
	{
		if (std::dynamic_pointer_cast<T>(*it)) 
		{
			m_Components.erase(it);
			return;
		}
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

void GameObject::Render() const
{
	for (auto& component : m_Components)
	{
		component->Render();
	}

}

void GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

