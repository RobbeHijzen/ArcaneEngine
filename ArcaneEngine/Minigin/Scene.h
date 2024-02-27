#pragma once
#include "SceneManager.h"


class GameObject;
class Scene final
{
	friend Scene& SceneManager::CreateScene(const std::string& name);
public:

	~Scene();
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;


	void Add(std::shared_ptr<GameObject> object);
	void Remove(std::shared_ptr<GameObject> object);
	void RemoveAll();


	void RemoveDeletedObjects();

	void Initialize();

	void Update();
	void FixedUpdate();
	void LateUpdate();
	void Render() const;

	std::shared_ptr<GameObject> GetRoot() { return m_Root; }

private: 
	explicit Scene(const std::string& name);

	std::string m_Name;
	std::shared_ptr<GameObject> m_Root{};
	std::vector<std::shared_ptr<GameObject>> m_GameObjects{};

	static unsigned int m_idCounter; 
};


