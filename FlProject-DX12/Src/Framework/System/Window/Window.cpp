#include "Window.h"

LRESULT WindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ���b�Z�[�W���ɏ�����I��
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(NULL);		// OS�ɑ΂��ďI����`����
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);		// ���b�Z�[�W�̃f�t�H���g����
		break;
	}

	return NULL;
}

bool Window::Create(const int clientWidth, const int clientHeight, const std::wstring& titleName, const std::wstring& windowClassName, const int32_t centerX, const int32_t centerY)
{
	// �C���X�^���X�n���h���擾
	auto hInst{ HINSTANCE{GetModuleHandle(NULL)} };

	//=====================================
	// ���C���E�B���h�E�쐬
	//=====================================

	// �E�B���h�E�N���X�̒�`
	auto wc{ WNDCLASSEX{} };
	wc.cbSize		 = sizeof(WNDCLASSEX);										// �\���̂̃T�C�Y
	wc.lpfnWndProc   = static_cast<WNDPROC>(WindowProcedure);				// �E�C���h�E�֐�
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));// �w�i�F 
	wc.lpszClassName = windowClassName.c_str();							// �E�B���h�E�N���X��
	wc.hInstance	 = hInst;												// �C���X�^���X�n���h��

	// �E�B���h�E�N���X�̓o�^
	if (!RegisterClassEx(&wc)) return false;

	// �E�B���h�E�̍쐬
	m_hWnd = CreateWindow(
		windowClassName.c_str(),							// �E�B���h�E�N���X��
		titleName.c_str(),									// �E�B���h�E�̃^�C�g��
		WS_OVERLAPPEDWINDOW - WS_THICKFRAME,				// �E�B���h�E�^�C�v��W���^�C�v��	
		centerX,											// �E�B���h�E�̈ʒu�i�w���W�j
		centerY,											// �E�B���h�E�̈ʒu�i�x���W�j						
		clientWidth,										// �E�B���h�E�̕�
		clientHeight,										// �E�B���h�E�̍���			
		nullptr,											// �e�E�B���h�E�̃n���h��
		nullptr,											// ���j���[�̃n���h��
		hInst,												// �C���X�^���X�n���h�� 
		this
	);

	if (m_hWnd == nullptr) return false;

	// �N���C�A���g�̃T�C�Y��ݒ�
	SetClientSize(clientWidth, clientHeight);

	// �E�B���h�E�̕\��
	ShowWindow(m_hWnd, SW_SHOW);

	//�E�B���h�E�̍X�V
	UpdateWindow(m_hWnd);

	return true;
}

bool Window::ProcessMessage()
{
	// ���b�Z�[�W�擾
	auto msg{ MSG{} };
	while (PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE))
	{
		// �I�����b�Z�[�W������
		if (msg.message == WM_QUIT) return false;

		//���b�Z�[�W����
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


// �E�B���h�E�̃N���C�A���g�̃T�C�Y���w��T�C�Y�ɂ���B
void Window::SetClientSize(const int w, const int h) const
{
	auto rcWnd{ RECT{} }, rcCli{ RECT{} };

	GetWindowRect(m_hWnd, &rcWnd); // �E�B���h�E��RECT�擾
	GetClientRect(m_hWnd, &rcCli); // �N���C�A���g�̈��RECT�擾

	// �E�B���h�E�̗]�����l���āA�N���C�A���g�̃T�C�Y���w��T�C�Y�ɂ���B
	MoveWindow(m_hWnd,
		rcWnd.left,// X���W
		rcWnd.top, // Y���W
		w + (rcWnd.right - rcWnd.left) - (rcCli.right - rcCli.left),
		h + (rcWnd.bottom - rcWnd.top) - (rcCli.bottom - rcCli.top),
		TRUE);
}

LRESULT Window::WindowProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	switch (message) {
		// �z�C�[���X�N���[����
	case WM_MOUSEWHEEL:
		m_mouseWheelVal = (short)HIWORD(wParam);
	break;
		// X�{�^���������ꂽ
	case WM_CLOSE:
		// �j��
		Release();
		break;
		// �E�B���h�E�j�����O
	case WM_DESTROY:
		RemoveProp(hWnd, L"GameWindowInstance");
		PostQuitMessage(NULL);
		break;
	default:
		// ���b�Z�[�W�̃f�t�H���g����
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return NULL;
}
