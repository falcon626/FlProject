#pragma once

class Application
{
public:

	void Execute();

	inline auto End() noexcept { m_isEnd = true; }

	inline const auto GetDeltaTime() const noexcept { return m_fpsController.GetDeltaTime(); }

	static auto& Instance() noexcept
	{
		static auto instance{ Application{} };
		return instance;
	}
private:

	Application();
	~Application();

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	void Draw();
	void Update();

	Window m_window{};
	FlFPSController m_fpsController{};

	bool m_isEnd{ false };
};