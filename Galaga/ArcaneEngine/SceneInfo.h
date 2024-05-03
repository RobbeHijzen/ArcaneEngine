#pragma once

namespace AE
{
	class Scene;

	class SceneInfo
	{
	public:

		virtual ~SceneInfo() = default;

		virtual void Load(Scene& scene) = 0;

	};
}
