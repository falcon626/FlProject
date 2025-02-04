#pragma once

#include "GraphicsDevice.h"

// <Heap:�q�[�v>
#include "Heap/Heap.h"
#include "Heap/RTVHeap/RTVHeap.h"
#include "Heap/CBVSRVUAVHeap/CBVSRVUAVHeap.h"
#include "Heap/DSVHeap/DSVHeap.h"

// <Buffer:�o�b�t�@>
#include "Buffer/Buffer.h"

// �萔�o�b�t�@�̃A���P�[�^�[
#include "Buffer/CBufferAllocater/CBufferAllocater.h"

// �萔�o�b�t�@�f�[�^
#include "Buffer/CBufferAllocater/CBufferData/CBufferData.h"

// �e�N�X�`��
#include "Buffer/Texture/Texture.h"

// �f�v�X�X�e���V��
#include "Buffer/DepthStencil/DepthStencil.h"

// ���b�V��
#include "Mesh/Mesh.h"

// ���f��
#include "Model/Model.h"

// �A�j���[�V����
#include "Animation/Animation.h"

#include "Shader/Shader.h"

class GraphicsAdministrator
{
public:

private:

	GraphicsAdministrator()  = default;
	~GraphicsAdministrator() = default;
};