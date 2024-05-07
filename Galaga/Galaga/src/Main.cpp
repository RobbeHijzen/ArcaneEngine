#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "ArcaneEngine.h"

#include "Level_01.h"
#include "Level_02.h"

using namespace AE;

void SetupScenes()
{
	//SceneManager::GetInstance().CreateScene(std::move(std::make_unique<Level_01>()));
	SceneManager::GetInstance().CreateScene(std::move(std::make_unique<Level_02>()));

}

int main(int, char* [])
{
	ArcaneEngine engine("../Resources/");
	engine.Run(SetupScenes);

	return 0;
}


