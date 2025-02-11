#pragma once

#include "GraphicsDevice.h"

// <Heap:ヒープ>
#include "Heap/Heap.h"
#include "Heap/RTVHeap/RTVHeap.h"
#include "Heap/CBVSRVUAVHeap/CBVSRVUAVHeap.h"
#include "Heap/DSVHeap/DSVHeap.h"

// <Buffer:バッファ>
#include "Buffer/Buffer.h"

// 定数バッファのアロケーター
#include "Buffer/CBufferAllocater/CBufferAllocater.h"

// 定数バッファデータ
#include "Buffer/CBufferAllocater/CBufferData/CBufferData.h"

// テクスチャ
#include "Buffer/Texture/Texture.h"

// デプスステンシル
#include "Buffer/DepthStencil/DepthStencil.h"

// メッシュ
#include "Mesh/Mesh.h"

// モデル
#include "Model/Model.h"

// アニメーション
#include "Animation/Animation.h"

#include "Shader/Shader.h"

class GraphicsAdministrator
{
public:

private:

	GraphicsAdministrator()  = default;
	~GraphicsAdministrator() = default;
};