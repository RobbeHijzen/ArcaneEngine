#pragma once
#include <memory>
#include "Transform.h"

class GameObject;
class BaseComponent
{
public:
	BaseComponent(std::weak_ptr<GameObject> gameObject);

	virtual ~BaseComponent() = default;
	BaseComponent(const BaseComponent& other) = delete;
	BaseComponent(BaseComponent&& other) = delete;
	BaseComponent& operator=(const BaseComponent& other) = delete;
	BaseComponent& operator=(BaseComponent&& other) = delete;


	virtual void Update() {};
	virtual void FixedUpdate() {};
	virtual void Render() const {};

	void SetRelativePosition(float x, float y, float z = 1.f) { m_RelativeTransform.SetPosition(x, y, z); }

protected:

	std::weak_ptr<GameObject> m_pParentGameObject;
	Transform m_RelativeTransform{};

};

