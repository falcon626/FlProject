#include "Application.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_  HINSTANCE, _In_ LPSTR, _In_ int)
{
	// メモリリークを知らせる
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// COM初期化
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		CoUninitialize();

		return NULL;
	}

	// mbstowcs_s関数で日本語対応にするために呼ぶ
	setlocale(LC_ALL, "japanese");

	//===================================================================
	// 実行
	//===================================================================
	Application::Instance().Execute();

	// COM解放
	CoUninitialize();

	return NULL;
}

Application::Application()
{
	static const int width = 1280;
	static const int height = 720;

	if (!m_window.Create(width, height, "FrameworkDX12", "Window"))
	{
		assert(NULL && "ウィンドウ作成失敗。");
		return;
	}

	if (!GraphicsDevice::Instance().Init(m_window.GetWndHandle(), width, height))
	{
		assert(NULL && "グラフィックスデバイス初期化失敗。");
		return;
	}
}

Application::~Application()
{
	m_window.Release();
}

void Application::Draw()
{
	// <Begin>
	{
		GraphicsDevice::Instance().PreDraw();
	}
	// </Begin>



	// <End>
	{
		//GraphicsDevice::Instance().ScreenFlip();
	}
	// </End>
}

void Application::Update()
{

}

void Application::Execute()
{
	auto debugLog{ std::make_unique<DebugLogger>() };

	ModelData modelData;
	modelData.Load("Assets/Model/Cube/Cube.gltf");

	Texture texture;
	texture.Load(&GraphicsDevice::Instance(), "Assets/Texture/SampleTex.jpg");

	Math::Matrix mWorld;

	RenderingSetting renderingSetting = {};
	renderingSetting.InputLayouts =
	{ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::COLOR,InputLayout::NORMAL,InputLayout::TANGENT };
	renderingSetting.Formats = { DXGI_FORMAT_R8G8B8A8_UNORM };

	Shader::Instance().Create(&GraphicsDevice::Instance(), L"SimpleShader",
		renderingSetting, { RangeType::CBV,RangeType::CBV, RangeType::SRV,RangeType::SRV,RangeType::SRV,RangeType::SRV });

	Math::Matrix mView = Math::Matrix::CreateTranslation(0, 0, 5);

	Math::Matrix mProj =
		DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), 1280.0f / 720.0f, 0.01f, 1000.0f);

	CBufferData::Camera cbCamera;
	cbCamera.mView = mView;
	cbCamera.mProj = mProj;

	Animator animator;
	animator.SetAnimation(modelData.GetAnimation(0));

	m_fpsController.Init();
	
	while (true)
	{
		m_fpsController.UpdateStartTime();

		animator.ProgressTime(modelData.WorkNodes(), 2.0f);

		// <描画関連処理>
		Draw();

		Shader::Instance().Begin(1280, 720);

		GraphicsDevice::Instance().GetCBufferAllocater()->BindAndAttachData(0, cbCamera);

		GraphicsDevice::Instance().GetCBufferAllocater()->BindAndAttachData(1, modelData.GetNodes()[0].m_mLocal * mWorld);

		Shader::Instance().DrawModel(modelData);

		GraphicsDevice::Instance().ScreenFlip();
		
		// <更新関連処理>
		Update();
		
		DEBUG_LOG(debugLog, "更新");

		std::string titleBar = "<Fps = " + std::to_string(m_fpsController.GetFPS()) + ">";
		SetWindowTextA(m_window.GetWndHandle(), titleBar.c_str());

		m_fpsController.Update();

		auto sz{ sizeof(bool) };

		auto i = sz;

		// <Exit:ゲームループ終了>
		if (!m_window.ProcessMessage()) End();
		if (m_isEnd) break;
	}
}
