#include "GraphicsDevice.h"

bool GraphicsDevice::Init(HWND hWnd, int w, int h)
{
	if (!CreateFactory())
	{
		assert(NULL && "ファクトリー作成失敗");
		return false;
	}

#ifdef _DEBUG
	EnableDebugLayer();
#endif

	if (!CreateDevice())
	{
		assert(NULL && "D3D12デバイス作成失敗");
		return false;
	}

	if (!CreateCommandList())
	{
		assert(NULL && "コマンドリストの作成失敗");
		return false;
	}

	if (!CreateSwapchain(hWnd, w, h))
	{
		assert(NULL && "スワップチェインの作成失敗");
		return false;
	}

	m_upRTVHeap = std::make_unique<RTVHeap>();
	if (!m_upRTVHeap->Create(this, HeapType::RTV, 100))
	{
		assert(NULL && "RTVヒープの作成失敗");
		return false;
	}

	m_upCBVSRVUAVHeap = std::make_unique<CBVSRVUAVHeap>();
	if (!m_upCBVSRVUAVHeap->Create(this, HeapType::CBVSRVUAV, Math::Vector3(100, 100, 100)))
	{
		assert(NULL && "CBVSRVUAVヒープの作成失敗");
		return false;
	}

	m_upCBufferAllocater = std::make_unique<CBufferAllocater>();
	m_upCBufferAllocater->Create(this, m_upCBVSRVUAVHeap.get());

	m_upDSVHeap = std::make_unique<DSVHeap>();
	if (!m_upDSVHeap->Create(this, HeapType::DSV, 100))
	{
		assert(NULL && "DSVヒープの作成失敗");
		return false;
	}

	m_upDepthStencil = std::make_unique<DepthStencil>();
	if (!m_upDepthStencil->Create(this, Math::Vector2(static_cast<float>(w), static_cast<float>(h))))
	{
		assert(NULL && "DepthStencilの作成失敗");
		return false;
	}

	if (!CreateSwapchainRTV())
	{
		assert(NULL && "スワップチェインRTVの作成失敗");
		return false;
	}

	if (!CreateFence())
	{
		assert(NULL && "フェンスの作成失敗");
		return false;
	}

	return true;
}

void GraphicsDevice::PreDraw()
{
	Prepare();

	GetCBVSRVUAVHeap()->SetHeap();

	GetCBufferAllocater()->ResetCurrentUseNumber();
}

void GraphicsDevice::Prepare()
{
	auto bbIdx{ m_pSwapChain->GetCurrentBackBufferIndex() };
	SetResourceBarrier(m_pSwapchainBuffers[bbIdx].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	auto rtvH{ m_upRTVHeap->GetCPUHandle(bbIdx) };
	auto dsvH{ m_upDSVHeap->GetCPUHandle(m_upDepthStencil->GetDSVNumber()) };
	m_pCmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	float clearColor[] { Def::FloatZero,Def::FloatOne,Def::FloatOne,Def::FloatOne };
	m_pCmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

	// デプスバッファのクリア
	m_upDepthStencil->ClearBuffer();
}

void GraphicsDevice::ScreenFlip()
{
	auto bbIdx{ m_pSwapChain->GetCurrentBackBufferIndex() };
	SetResourceBarrier(m_pSwapchainBuffers[bbIdx].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	m_pCmdList->Close();

	ID3D12CommandList* cmdlists[] = { m_pCmdList.Get() };
	m_pCmdQueue->ExecuteCommandLists(1, cmdlists);

	WaitForCommandQueue();

	m_pCmdAllocator->Reset();								// コマンドアロケーターの初期化
	m_pCmdList->Reset(m_pCmdAllocator.Get(), nullptr);		// コマンドリストの初期化

	m_pSwapChain->Present(1, 0);
}

void GraphicsDevice::WaitForCommandQueue()
{
	m_pCmdQueue->Signal(m_pFence.Get(), ++m_fenceVal);

	if (m_pFence->GetCompletedValue() != m_fenceVal)
	{
		auto event{ CreateEvent(nullptr, false, false, nullptr) };		// イベントハンドルの取得
		if (!event) assert(NULL && "イベントエラー、アプリケーションを終了します");
		m_pFence->SetEventOnCompletion(m_fenceVal, event);
		WaitForSingleObject(event, INFINITE);		// イベントが発生するまで待ち続ける
		CloseHandle(event);							// イベントハンドルを閉じる
	}
}

bool GraphicsDevice::CreateFactory()
{
	auto flagsDXGI{ Def::UIntZero };
	flagsDXGI |= DXGI_CREATE_FACTORY_DEBUG;
	auto hr{ CreateDXGIFactory2(flagsDXGI, IID_PPV_ARGS(m_pDxgiFactory.GetAddressOf())) };

	if (FAILED(hr)) return false;

	return true;
}

bool GraphicsDevice::CreateDevice()
{
	auto pSelectAdapter{ ComPtr<IDXGIAdapter>{nullptr} };
	auto pAdapters     { std::vector<ComPtr<IDXGIAdapter>>{} };
	auto descs		   { std::vector<DXGI_ADAPTER_DESC>{} };

	// 使用中PCにあるGPUドライバーを検索して、あれば格納する
	for (auto index{ Def::UIntZero }; true; ++index)
	{
		pAdapters.push_back(nullptr);
		HRESULT ret = m_pDxgiFactory->EnumAdapters(index, &pAdapters[index]);

		if (ret == DXGI_ERROR_NOT_FOUND) break;

		descs.push_back({});
		pAdapters[index]->GetDesc(&descs[index]);
	}

	auto gpuTier{ GPUTier::Kind };

	// 優先度の高いGPUドライバーを使用する
	for (auto i{ Def::IntZero }; i < descs.size(); ++i)
	{
		if (std::wstring(descs[i].Description).find(L"NVIDIA") != std::wstring::npos)
		{
			pSelectAdapter = pAdapters[i];
			break;
		}
		else if (std::wstring(descs[i].Description).find(L"Amd") != std::wstring::npos)
		{
			if (gpuTier > GPUTier::Amd)
			{
				pSelectAdapter = pAdapters[i];
				gpuTier = GPUTier::Amd;
			}
		}
		else if (std::wstring(descs[i].Description).find(L"Intel") != std::wstring::npos)
		{
			if (gpuTier > GPUTier::Intel)
			{
				pSelectAdapter = pAdapters[i];
				gpuTier = GPUTier::Intel;
			}
		}
		else if (std::wstring(descs[i].Description).find(L"Arm") != std::wstring::npos)
		{
			if (gpuTier > GPUTier::Arm)
			{
				pSelectAdapter = pAdapters[i];
				gpuTier = GPUTier::Arm;
			}
		}
		else if (std::wstring(descs[i].Description).find(L"Qualcomm") != std::wstring::npos)
		{
			if (gpuTier > GPUTier::Qualcomm)
			{
				pSelectAdapter = pAdapters[i];
				gpuTier = GPUTier::Qualcomm;
			}
		}
	}

	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	// Direct3Dデバイスの初期化
	D3D_FEATURE_LEVEL featureLevel;
	for (auto lv : levels)
	{
		if (D3D12CreateDevice(pSelectAdapter.Get(), lv, IID_PPV_ARGS(&m_pDevice)) == S_OK)
		{
			featureLevel = lv;
			break;		// 生成可能なバージョンが見つかったらループ打ち切り
		}
	}

	return true;
}

bool GraphicsDevice::CreateCommandList()
{
	auto hr = m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCmdAllocator));

	if (FAILED(hr))
	{
		return false;
	}

	hr = m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCmdAllocator.Get(), nullptr, IID_PPV_ARGS(&m_pCmdList));

	if (FAILED(hr))
	{
		return false;
	}

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;				// タイムアウトなし
	cmdQueueDesc.NodeMask = 0;										// アダプターを1つしか使わないときは0でいい
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;	// プライオリティは特に指定なし
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;				// コマンドリストと合わせる

	// キュー生成
	hr = m_pDevice->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&m_pCmdQueue));

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool GraphicsDevice::CreateSwapchain(HWND hWnd, int width, int height)
{
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
	swapchainDesc.Width = width;
	swapchainDesc.Height = height;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.BufferCount = 2;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;		// フリップ後は速やかに破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// ウィンドウとフルスクリーン切り替え可能

	auto hr = m_pDxgiFactory->CreateSwapChainForHwnd(m_pCmdQueue.Get(), hWnd, &swapchainDesc, nullptr, nullptr, (IDXGISwapChain1**)m_pSwapChain.GetAddressOf());

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool GraphicsDevice::CreateSwapchainRTV()
{
	for (int i = 0; i < (int)m_pSwapchainBuffers.size(); ++i)
	{
		auto hr = m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pSwapchainBuffers[i]));

		if (FAILED(hr))
		{
			return false;
		}

		m_upRTVHeap->CreateRTV(m_pSwapchainBuffers[i].Get());
	}

	return true;
}

bool GraphicsDevice::CreateFence()
{
	auto hr = m_pDevice->CreateFence(m_fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence));

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void GraphicsDevice::SetResourceBarrier(ID3D12Resource* pResource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Transition.pResource = pResource;
	barrier.Transition.StateAfter = after;
	barrier.Transition.StateBefore = before;
	m_pCmdList->ResourceBarrier(1, &barrier);
}

void GraphicsDevice::EnableDebugLayer()
{
	auto pDebugLayer{ static_cast<ID3D12Debug*>(nullptr) };

	D3D12GetDebugInterface(IID_PPV_ARGS(&pDebugLayer));
	pDebugLayer->EnableDebugLayer(); // デバッグレイヤーを有効にする
	pDebugLayer->Release();
}

void GraphicsDevice::WaitForGPU()
{
	// フェンスが初期化されていない場合は作成
	if (!m_pFence)
	{
		auto hr{ m_pDevice->CreateFence(NULL, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence)) };
		if (FAILED(hr))
		{
			assert(false && "Failed To Create Fence");
			return;
		}
		m_fenceVal = Def::ULongLongOne;
	}

	// イベントハンドルを初期化（必要な場合のみ）
	if (m_fenceEvent == nullptr)
	{
		m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (m_fenceEvent == nullptr)
		{
			assert(false && "Failed To Create Fence Event");
			return;
		}
	}

	// コマンドキューにフェンスをシグナル
	{
		auto hr{ m_pCmdQueue->Signal(m_pFence.Get(), m_fenceVal) };
		if (FAILED(hr))
		{
			assert(false && "Failed To Signal Command Queue");
			return;
		}
	}

	// フェンスが完了していない場合、イベントを待機
	if (m_pFence->GetCompletedValue() < m_fenceVal)
	{
		auto hr{ m_pFence->SetEventOnCompletion(m_fenceVal, m_fenceEvent) };
		if (FAILED(hr))
		{
			assert(false && "Failed To Set Fence Completion Event");
			return;
		}

		WaitForSingleObject(m_fenceEvent, INFINITE);
	}

	++m_fenceVal;
}

GraphicsDevice::~GraphicsDevice()
{
	WaitForGPU();

	if (m_fenceEvent)
	{
		CloseHandle(m_fenceEvent);
		m_fenceEvent = nullptr;
	}
}
