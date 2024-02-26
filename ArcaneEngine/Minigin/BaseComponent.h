#pragma once
#include <memory>
#include "Transform.h"


class GameObject;

class BaseComponent
{
public:
	BaseComponent(GameObject* pParent);

	virtual ~BaseComponent() = default;
	BaseComponent(const BaseComponent& other) = delete;
	BaseComponent(BaseComponent&& other) = delete;
	BaseComponent& operator=(const BaseComponent& other) = delete;
	BaseComponent& operator=(BaseComponent&& other) = delete;


	virtual void Update() {};
	virtual void FixedUpdate() {};
	virtual void LateUpdate() {};
	virtual void Render() const {};

	inline void SetLocalPosition(float x, float y, float z = 0.f) { m_LocalTransform.SetPosition(x, y, z); }

protected:

	GameObject* GetParent() const { return m_pParent; }

	Transform m_LocalTransform{};

private:

	GameObject* m_pParent;
};

