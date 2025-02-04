#include "FPSController.h"

void FlFPSController::Init()
{
	QueryPerformanceFrequency(&m_frequency);
	QueryPerformanceCounter(&m_fpsMonitorBeginTime);
	m_frameStartTime = m_fpsMonitorBeginTime;
}

void FlFPSController::UpdateStartTime()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	m_deltaTime = static_cast<float>(currentTime.QuadPart - m_frameStartTime.QuadPart) / m_frequency.QuadPart;
	m_frameStartTime = currentTime;
}

void FlFPSController::Update()
{
	Control();
	Monitoring();
}

void FlFPSController::Control() const
{
	LARGE_INTEGER frameProcessEndTime;
	QueryPerformanceCounter(&frameProcessEndTime);

	// Time That Should Elapse In One Frame
	auto timePerFrame{ static_cast<double>(m_frequency.QuadPart) / m_maxFps };

	if (frameProcessEndTime.QuadPart - m_frameStartTime.QuadPart < timePerFrame) 
	{
		DWORD sleepTime = static_cast<DWORD>((timePerFrame - (frameProcessEndTime.QuadPart - m_frameStartTime.QuadPart)) * 1000 / m_frequency.QuadPart);
		Sleep(sleepTime);
	}
}

void FlFPSController::Monitoring()
{
	// FPS Measure 0.5 Seconds Interval
	constexpr auto FpsRefreshFrame{ 0.5 }; // Second

	m_fpsCnt++;

	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	auto elapsedTime = static_cast<double>(currentTime.QuadPart - m_fpsMonitorBeginTime.QuadPart) / m_frequency.QuadPart;

	if (elapsedTime >= FpsRefreshFrame) {
		m_nowfps = static_cast<float>(m_fpsCnt / elapsedTime);

		m_fpsMonitorBeginTime = currentTime;
		m_fpsCnt = Def::ULongZero;
	}
}