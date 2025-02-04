#pragma once

class FlFPSController
{
public:
	void Init();
	void UpdateStartTime();
	void Update();

	const auto GetDeltaTime() const noexcept { return m_deltaTime; }
	const auto GetFPS()       const noexcept { return m_nowfps; }

private:
	void Control() const;
	void Monitoring();

	LARGE_INTEGER m_frequency{ NULL };
	LARGE_INTEGER m_frameStartTime{ NULL };
	LARGE_INTEGER m_fpsMonitorBeginTime{ NULL };
	DWORD m_maxFps = 10000;
	DWORD m_fpsCnt = Def::ULongZero;
	float m_nowfps = Def::FloatZero;
	float m_deltaTime = Def::FloatZero;

	const DWORD EcoFps = 10;
	static constexpr DWORD kSecond = 1000;
};