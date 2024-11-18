#pragma once

class Window
{
public:

	HWND GetWndHandle() const { return m_hWnd; }

	bool Create(const int clientWidth, const int clientHeight, const std::wstring& titleName, const std::wstring& windowClassName, const int32_t centerX = NULL, const int32_t centerY = NULL);

	bool ProcessMessage();

	const auto GetMouseWheelVal() const noexcept { return m_mouseWheelVal; }

	// ‰ð•ú
	void Release();

private:

	void SetClientSize(const int w, const int h) const;

	LRESULT CALLBACK WindowProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam);

	HWND	m_hWnd{};

	int32_t m_mouseWheelVal{ NULL };
};