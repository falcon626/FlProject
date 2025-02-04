#include "inc_TextureShader.hlsli"

// ==========================================
// テクスチャ用 HLSL (TextureShader.hlsl)
// ==========================================

// ピクセルシェーダー
float4 PSMain(PS_INPUT input) : SV_TARGET
{
    // テクスチャから色を取得して返す
    return gTexture.Sample(gSampler, input.TexCoord);
}