#include "Texture.h"

bool Texture::Load(GraphicsDevice* pGraphicsDevice, const std::string& filePath)
{
	m_pGraphicsDevice = pGraphicsDevice;

	wchar_t wFilePath[128];
	MultiByteToWideChar(CP_ACP, 0, filePath.c_str(), -1, wFilePath, _countof(wFilePath));

	auto metadata{ DirectX::TexMetadata {} };
	auto scratchImage{ DirectX::ScratchImage {} };

	const DirectX::Image* pImage{ nullptr };

	auto hr{ DirectX::LoadFromWICFile(wFilePath, DirectX::WIC_FLAGS_NONE, &metadata, scratchImage) };

	if (FAILED(hr))
	{
		assert("�e�N�X�`���̓ǂݍ��ݎ��s");
		return false;
	}

	pImage = scratchImage.GetImage(0, 0, 0);

	D3D12_HEAP_PROPERTIES heapprop = {};
	heapprop.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	resDesc.Format = metadata.format;
	resDesc.Width = static_cast<UINT64>(metadata.width);
	resDesc.Height = static_cast<UINT>(metadata.height);
	resDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
	resDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
	resDesc.SampleDesc.Count = Def::UIntOne;

	hr = m_pGraphicsDevice->GetDevice()->CreateCommittedResource(&heapprop, D3D12_HEAP_FLAG_NONE, &resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_pBuffer));

	if (FAILED(hr))
	{
		assert(false && "�e�N�X�`���o�b�t�@�쐬���s");
		return false;
	}

	hr = m_pBuffer->WriteToSubresource(0, nullptr, pImage->pixels, (UINT)pImage->rowPitch, (UINT)pImage->slicePitch);

	if (FAILED(hr))
	{
		assert(false && "�o�b�t�@�Ƀe�N�X�`���f�[�^�̏������ݎ��s");
		return false;
	}

	m_srvNumber = m_pGraphicsDevice->GetCBVSRVUAVHeap()->CreateSRV(m_pBuffer.Get());

	return true;
}

void Texture::Set(int index)
{
	m_pGraphicsDevice->GetCmdList()->SetGraphicsRootDescriptorTable
	(index, m_pGraphicsDevice->GetCBVSRVUAVHeap()->GetGPUHandle(m_srvNumber));
}