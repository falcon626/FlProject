// ==========================================
// テクスチャ用 HLSLI (TextureShader.hlsli)
// ==========================================

// 定数バッファ
cbuffer TransformBuffer : register(b2)
{
    float4x4 gWorldViewProj; // ワールド・ビュー・プロジェクション行列
}

// 入力構造体（頂点シェーダー用）
struct VS_INPUT
{
    float3 Position : POSITION; // 頂点の位置
    float2 TexCoord : TEXCOORD; // テクスチャ座標
};

// 出力構造体（ピクセルシェーダー用）
struct PS_INPUT
{
    float4 Position : SV_POSITION; // システム座標空間の位置
    float2 TexCoord : TEXCOORD; // テクスチャ座標
};

// テクスチャとサンプラー
Texture2D gTexture : register(t4); // テクスチャ
SamplerState gSampler : register(s5); // サンプラー

// 頂点シェーダー
PS_INPUT VSMain(VS_INPUT input)
{
    PS_INPUT output;
    // ワールド・ビュー・プロジェクション行列を適用
    output.Position = mul(float4(input.Position, 1.0), gWorldViewProj);
    output.TexCoord = input.TexCoord;
    return output;
}