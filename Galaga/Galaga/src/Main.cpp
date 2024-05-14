#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "ArcaneEngine.h"

#include "StartScreenLevel.h"
#include "GameLevel.h"

using namespace AE;

void SetupScenes()
{
	SceneManager::GetInstance().CreateScene(std::move(std::make_unique<StartScreenLevel>()));
	SceneManager::GetInstance().CreateScene(std::move(std::make_unique<GameLevel>()));

}

int main(int, char* [])
{
	ArcaneEngine engine("../Resources/");
	engine.Run(SetupScenes);

	return 0;
}


