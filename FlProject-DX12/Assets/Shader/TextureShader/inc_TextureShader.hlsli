// ==========================================
// �e�N�X�`���p HLSLI (TextureShader.hlsli)
// ==========================================

// �萔�o�b�t�@
cbuffer TransformBuffer : register(b2)
{
    float4x4 gWorldViewProj; // ���[���h�E�r���[�E�v���W�F�N�V�����s��
}

// ���͍\���́i���_�V�F�[�_�[�p�j
struct VS_INPUT
{
    float3 Position : POSITION; // ���_�̈ʒu
    float2 TexCoord : TEXCOORD; // �e�N�X�`�����W
};

// �o�͍\���́i�s�N�Z���V�F�[�_�[�p�j
struct PS_INPUT
{
    float4 Position : SV_POSITION; // �V�X�e�����W��Ԃ̈ʒu
    float2 TexCoord : TEXCOORD; // �e�N�X�`�����W
};

// �e�N�X�`���ƃT���v���[
Texture2D gTexture : register(t4); // �e�N�X�`��
SamplerState gSampler : register(s5); // �T���v���[

// ���_�V�F�[�_�[
PS_INPUT VSMain(VS_INPUT input)
{
    PS_INPUT output;
    // ���[���h�E�r���[�E�v���W�F�N�V�����s���K�p
    output.Position = mul(float4(input.Position, 1.0), gWorldViewProj);
    output.TexCoord = input.TexCoord;
    return output;
}