#pragma once

class RTVHeap;
class CBVSRVUAVHeap;
class CBufferAllocater;
class DSVHeap;
class DepthStencil;

class GraphicsDevice
{
public:

	// <Initialize:����������>�@<Arg:�E�B���h�E�n���h���A���A��>
	bool Init(HWND hWnd, int w, int h);

	// <Pre:�`�掖�O��������>
	void PreDraw();

	// <Swap:�؂�ւ�>
	void ScreenFlip();

	// <Synchronization:�R�}���h���C������>
	void WaitForCommandQueue();

	// <Getter:�f�o�C�X>
	ID3D12Device8* GetDevice()const { return m_pDevice.Get(); }

	// <Getter:�R�}���h���C��>
	ID3D12GraphicsCommandList6* GetCmdList()const { return m_pCmdList.Get(); }

	// <Getter:�萔�o�b�t�@�r���[,�V�F�[�_�[���\�[�X�r���[,�A���I�[�_�[�A�N�Z�X�r���[�̃q�[�v>
	CBVSRVUAVHeap* GetCBVSRVUAVHeap()const { return m_upCBVSRVUAVHeap.get(); }

	// <Getter:�萔�o�b�t�@�A���P�[�^�[>
	CBufferAllocater* GetCBufferAllocater()const { return m_upCBufferAllocater.get(); }

	// <Getter:�[�x,�X�e���V���r���[�q�[�v>
	DSVHeap* GetDSVHeap()const { return m_upDSVHeap.get(); }

private:

	// <Pre:�`�掖�O����>
	void Prepare();

	// <Create:�t�@�N�g���[�쐬>
	bool CreateFactory();

	// <Create:�f�o�C�X�쐬>
	bool CreateDevice();

	// <Create:�R�}���h���X�g�쐬>
	bool CreateCommandList();

	// <Create:�X���b�v�`�F�C���쐬>�@<Arg:�E�B���h�E�n���h���A���A��>
	bool CreateSwapchain(HWND hWnd, int width, int height);

	// <Create:�X���b�v�`�F�C��RTV�쐬>
	bool CreateSwapchainRTV();

	// <Create:�t�F���X�쐬>
	bool CreateFence();

	// <Setter:�o�b�t�@�̈�����ύX>�@<Arg:�w��o�b�t�@�A���݂̏�ԁA�V�������>
	void SetResourceBarrier(ID3D12Resource* pResource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

	// <Debug:�f�o�b�N���C���[�K��>
	void EnableDebugLayer();

	void WaitForGPU();

	enum class GPUTier : size_t
	{
		NVIDIA,
		Amd,
		Intel,
		Arm,
		Qualcomm,
		Kind,
	};

	enum class SwapBafferNum : size_t
	{ Zero, One, Max };

	ComPtr<ID3D12Device8>					m_pDevice;
	ComPtr<IDXGIFactory6>					m_pDxgiFactory;

	ComPtr<ID3D12CommandAllocator>			m_pCmdAllocator;
	ComPtr<ID3D12GraphicsCommandList6>		m_pCmdList;
	ComPtr<ID3D12CommandQueue>				m_pCmdQueue;

	ComPtr<IDXGISwapChain4>						 m_pSwapChain;
	std::array<ComPtr<ID3D12Resource>, 
		static_cast<size_t>(SwapBafferNum::Max)> m_pSwapchainBuffers;

	ComPtr<ID3D12Fence>					m_pFence;
	uint64_t							m_fenceVal{ Def::ULongLongZero };
	HANDLE                              m_fenceEvent{ nullptr };

	std::unique_ptr<RTVHeap>			m_upRTVHeap;
	std::unique_ptr<CBVSRVUAVHeap>		m_upCBVSRVUAVHeap;
	std::unique_ptr<CBufferAllocater>	m_upCBufferAllocater;
	std::unique_ptr<DSVHeap>			m_upDSVHeap;
	std::unique_ptr<DepthStencil>		m_upDepthStencil;

	GraphicsDevice()  = default;
	~GraphicsDevice();
public:
	static GraphicsDevice& Instance()
	{
		static GraphicsDevice instance;
		return instance;
	}
};