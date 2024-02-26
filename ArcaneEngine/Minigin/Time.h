#pragma once

#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

struct Time final: public Singleton<Time>
{
	float deltaTime{};
	float fixedTimeStep{};
	int msPerFrame{};
	float fps{};
};

