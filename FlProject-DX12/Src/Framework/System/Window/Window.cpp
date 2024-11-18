#include "Window.h"

LRESULT WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// メッセージ毎に処理を選択
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(NULL);		// OSに対して終了を伝える
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);		// メッセージのデフォルト処理
		break;
	}

	return NULL;
}

bool Window::Create(const int clientWidth, const int clientHeight, const std::wstring& titleName, const std::wstring& windowClassName, const int32_t centerX, const int32_t centerY)
{
	// インスタンスハンドル取得
	auto hInst{ HINSTANCE{GetModuleHandle(NULL)} };

	//=====================================
	// メインウィンドウ作成
	//=====================================

	// ウィンドウクラスの定義
	auto wc{ WNDCLASSEX{} };
	wc.cbSize		 = sizeof(WNDCLASSEX);										// 構造体のサイズ
	wc.lpfnWndProc   = static_cast<WNDPROC>(WindowProcedure);				// ウインドウ関数
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));// 背景色 
	wc.lpszClassName = windowClassName.c_str();							// ウィンドウクラス名
	wc.hInstance	 = hInst;												// インスタンスハンドル

	// ウィンドウクラスの登録
	if (!RegisterClassEx(&wc)) return false;

	// ウィンドウの作成
	m_hWnd = CreateWindow(
		windowClassName.c_str(),							// ウィンドウクラス名
		titleName.c_str(),									// ウィンドウのタイトル
		WS_OVERLAPPEDWINDOW - WS_THICKFRAME,				// ウィンドウタイプを標準タイプに	
		centerX,											// ウィンドウの位置（Ｘ座標）
		centerY,											// ウィンドウの位置（Ｙ座標）						
		clientWidth,										// ウィンドウの幅
		clientHeight,										// ウィンドウの高さ			
		nullptr,											// 親ウィンドウのハンドル
		nullptr,											// メニューのハンドル
		hInst,												// インスタンスハンドル 
		this
	);

	if (m_hWnd == nullptr) return false;

	// クライアントのサイズを設定
	SetClientSize(clientWidth, clientHeight);

	// ウィンドウの表示
	ShowWindow(m_hWnd, SW_SHOW);

	//ウィンドウの更新
	UpdateWindow(m_hWnd);

	return true;
}

bool Window::ProcessMessage()
{
	// メッセージ取得
	auto msg{ MSG{} };
	while (PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE))
	{
		// 終了メッセージがきた
		if (msg.message == WM_QUIT) return false;

		//メッセージ処理
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

void Window::Release()
{
	if (m_hWnd) 
	{
		DestroyWindow(m_hWnd);
		m_hWnd = nullptr;
	}
}


// ウィンドウのクライアントのサイズを指定サイズにする。
void Window::SetClientSize(const int w, const int h) const
{
	auto rcWnd{ RECT{} }, rcCli{ RECT{} };

	GetWindowRect(m_hWnd, &rcWnd); // ウィンドウのRECT取得
	GetClientRect(m_hWnd, &rcCli); // クライアント領域のRECT取得

	// ウィンドウの余白を考えて、クライアントのサイズを指定サイズにする。
	MoveWindow(m_hWnd,
		rcWnd.left,// X座標
		rcWnd.top, // Y座標
		w + (rcWnd.right - rcWnd.left) - (rcCli.right - rcCli.left),
		h + (rcWnd.bottom - rcWnd.top) - (rcCli.bottom - rcCli.top),
		TRUE);
}

LRESULT Window::WindowProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	switch (message) {
		// ホイールスクロール時
	case WM_MOUSEWHEEL:
		m_mouseWheelVal = (short)HIWORD(wParam);
	break;
		// Xボタンが押された
	case WM_CLOSE:
		// 破棄
		Release();
		break;
		// ウィンドウ破棄直前
	case WM_DESTROY:
		RemoveProp(hWnd, L"GameWindowInstance");
		PostQuitMessage(NULL);
		break;
	default:
		// メッセージのデフォルト処理
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return NULL;
}
