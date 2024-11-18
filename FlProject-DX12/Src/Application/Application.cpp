#include "Application.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_  HINSTANCE, _In_ LPSTR, _In_ int)
{
	// ���������[�N��m�点��
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// COM������
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		CoUninitialize();

		return 0;
	}

	// mbstowcs_s�֐��œ��{��Ή��ɂ��邽�߂ɌĂ�
	setlocale(LC_ALL, "japanese");

	//===================================================================
	// ���s
	//===================================================================
	Application::Instance().Execute();

	// COM���
	CoUninitialize();

	return 0;
}

Application::Application()
{
	static const int width = 1280;
	static const int height = 720;

	if (!m_window.Create(width, height, L"FrameworkDX12", L"Window"))
	{
		assert(0 && "�E�B���h�E�쐬���s�B");
		return;
	}
	std::filesystem::path str;
}

Application::~Application()
{

}

void Application::Execute()
{
	while (true)
	{
		if (!m_window.ProcessMessage())
		{
			break;
		}
	}
}
