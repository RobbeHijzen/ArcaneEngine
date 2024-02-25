#pragma once
#include <string>
#include <functional>


class ArcaneEngine
{
public:
	explicit ArcaneEngine(const std::string& dataPath);
	~ArcaneEngine();
	void Run(const std::function<void()>& load);

	ArcaneEngine(const ArcaneEngine& other) = delete;
	ArcaneEngine(ArcaneEngine&& other) = delete;
	ArcaneEngine& operator=(const ArcaneEngine& other) = delete;
	ArcaneEngine& operator=(ArcaneEngine&& other) = delete;
};
