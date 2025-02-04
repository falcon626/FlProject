#pragma once

#include "Model.h"

class ModelLoader
{
public:

	/// <summary>
	/// ���f���̃��[�h
	/// </summary>
	/// <param name="filepath">�t�@�C���p�X</param>
	/// <param name="nodes">�m�[�h���</param>
	/// <returns>����������true</returns>
	bool Load(std::string filepath, ModelData& model);

private:

	/// <summary>
	/// ���
	/// </summary>
	/// <param name="pScene">���f���V�[���̃|�C���^</param>
	/// <param name="pMesh">���b�V���̃|�C���^</param>
	/// <param name="pMaterial">�}�e���A���̃|�C���^</param>
	/// <param name="dirPath">�f�B���N�g���p�X</param>
	/// <returns>���b�V���|�C���^</returns>
	std::shared_ptr<Mesh> Parse(const aiScene* pScene, const aiMesh* pMesh,
		const aiMaterial* pMaterial, const std::string& dirPath);

	/// <summary>
	/// �}�e���A���̉��
	/// </summary>
	/// <param name="pMaterial">�}�e���A���̃|�C���^</param>
	/// <param name="dirPath">�f�B���N�g���p�X</param>
	/// <returns>�}�e���A�����</returns>
	const Material ParseMaterial(const aiMaterial* pMaterial, const std::string& dirPath);
};