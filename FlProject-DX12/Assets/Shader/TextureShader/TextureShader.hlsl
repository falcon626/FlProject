#include "inc_TextureShader.hlsli"

// ==========================================
// �e�N�X�`���p HLSL (TextureShader.hlsl)
// ==========================================

// �s�N�Z���V�F�[�_�[
float4 PSMain(PS_INPUT input) : SV_TARGET
{
    // �e�N�X�`������F���擾���ĕԂ�
    return gTexture.Sample(gSampler, input.TexCoord);
}