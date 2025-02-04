#pragma once

enum class DepthStencilFormat
{
	DepthLowQuality = DXGI_FORMAT_R16_TYPELESS,
	DepthHighQuality = DXGI_FORMAT_R32_TYPELESS,
	DepthHighQualityAndStencil = DXGI_FORMAT_R24G8_TYPELESS,
};

class DepthStencil :public Buffer
{
public:

	// <Create:�[�x�e�N�X�`���쐬>�@<Arg:GraphicsDevice�̃|�C���^,�𑜓x,�[�x�e�N�X�`���̃t�H�[�}�b�g>
	bool Create(GraphicsDevice* pGraphicsDevice, const Math::Vector2& resolution,
		DepthStencilFormat format = DepthStencilFormat::DepthHighQuality);

	// <Clear:�[�x�o�b�t�@��������>
	void ClearBuffer();

	// <Getter:DSV�ԍ�>
	inline const auto GetDSVNumber() const noexcept { return m_dsvNumber; }

private:
	uint32_t m_dsvNumber{ Def::UIntZero };
};