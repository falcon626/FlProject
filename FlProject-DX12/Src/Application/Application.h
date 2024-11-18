#pragma once

#include "../Framework/System/Window/Window.h"

class Application
{
public:

	void Execute();

	static auto& Instance()
	{
		Application instance;
		return instance;
	}
private:

	Application();
	~Application();

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	Window m_window;
};