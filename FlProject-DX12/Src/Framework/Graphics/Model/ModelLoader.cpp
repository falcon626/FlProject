#include "ModelLoader.h"

bool ModelLoader::Load(std::string filepath, ModelData& model)
{
	auto importer{ Assimp::Importer{} };

	auto flag{ Def::IntZero };
	//flag |= aiProcess_CalcTangentSpace;
	//flag |= aiProcess_Triangulate;
	//flag |= aiProcess_ConvertToLeftHanded;
	flag |= aiProcess_Triangulate | aiProcess_FlipUVs;
	//flag |= aiProcess_GenSmoothNormals;
	//flag |= aiProcess_PreTransformVertices;
	//flag |= aiProcess_RemoveRedundantMaterials;
	//flag |= aiProcess_GenUVCoords;
	//flag |= aiProcess_OptimizeMeshes;

	const auto pScene{ importer.ReadFile(filepath, flag) };

	if (pScene == nullptr)
	{
		assert(false && "���f���̃t�@�C����������܂���");
		return false;
	}

	auto& nodes{ model.WorkNodes() };
	nodes.resize(pScene->mNumMeshes);

	auto dirPath{ GetDirFromPath(filepath) };

	for (auto i{ Def::UIntZero }; i < pScene->mNumMeshes; ++i)
	{
		auto pMesh    { pScene->mMeshes[i] };
		auto pMaterial{ pScene->mMaterials[i] };
		nodes[i].spMesh = Parse(pScene, pMesh, pMaterial, dirPath);
	}

	decltype(auto) spAnimationDatas{ model.WorkAnimation() };

	for (auto i{ Def::UIntZero }; i < pScene->mNumAnimations; ++i)
	{
		auto pAnimation { pScene->mAnimations[i] };

		auto spAnimaData{ std::make_shared<AnimationData>() };
		spAnimaData->m_name    = pAnimation->mName.C_Str();
		spAnimaData->m_maxTime = static_cast<float>(pAnimation->mDuration);

		spAnimaData->m_channels.resize(pAnimation->mNumChannels);

		for (auto j{ Def::UIntZero }; j < pAnimation->mNumChannels; ++j)
		{
			auto& srcChannel{ spAnimaData->m_channels[j] };
			auto dstChannel { pAnimation->mChannels[j] };

			for (auto k{ Def::UIntZero }; k < dstChannel->mNumPositionKeys; ++k)
			{
				auto translation{ AnimKeyVector3{} };
				translation.m_time = static_cast<float>(dstChannel->mPositionKeys[k].mTime);

				translation.m_vec.x = dstChannel->mPositionKeys[k].mValue.x;
				translation.m_vec.y = dstChannel->mPositionKeys[k].mValue.y;
				translation.m_vec.z = dstChannel->mPositionKeys[k].mValue.z;

				srcChannel.m_translations.emplace_back(translation);
			}

			for (auto k{ Def::UIntZero }; k < dstChannel->mNumRotationKeys; ++k)
			{
				auto rotation{ AnimKeyQuaternion{} };
				rotation.m_time = static_cast<float>(dstChannel->mRotationKeys[k].mTime);

				rotation.m_quat.x = dstChannel->mRotationKeys[k].mValue.x;
				rotation.m_quat.y = dstChannel->mRotationKeys[k].mValue.y;
				rotation.m_quat.z = dstChannel->mRotationKeys[k].mValue.z;
				rotation.m_quat.w = dstChannel->mRotationKeys[k].mValue.w;

				srcChannel.m_rotations.emplace_back(rotation);
			}

			for (auto k{ Def::UIntZero }; k < dstChannel->mNumScalingKeys; ++k)
			{
				auto scale{ AnimKeyVector3{} };
				scale.m_time = static_cast<float>(dstChannel->mScalingKeys[k].mTime);

				scale.m_vec.x = dstChannel->mScalingKeys[k].mValue.x;
				scale.m_vec.y = dstChannel->mScalingKeys[k].mValue.y;
				scale.m_vec.z = dstChannel->mScalingKeys[k].mValue.z;

				srcChannel.m_scales.emplace_back(scale);
			}
		}

		spAnimationDatas.emplace_back(spAnimaData);
	}

	return true;
}

std::shared_ptr<Mesh> ModelLoader::Parse(const aiScene* pScene, const aiMesh* pMesh,
	const aiMaterial* pMaterial, const std::string& dirPath)
{
	std::vector<MeshVertex> vertices;
	std::vector<MeshFace> faces;

	vertices.resize(pMesh->mNumVertices);

	for (auto i{ Def::UIntZero }; i < pMesh->mNumVertices; ++i)
	{
		vertices[i].Position.x = pMesh->mVertices[i].x;
		vertices[i].Position.y = pMesh->mVertices[i].y;
		vertices[i].Position.z = pMesh->mVertices[i].z;

		if (pMesh->HasTextureCoords(0))
		{
			vertices[i].UV.x = static_cast<float>(pMesh->mTextureCoords[0][i].x);
			vertices[i].UV.y = static_cast<float>(pMesh->mTextureCoords[0][i].y);

			// UV�𔽓]������
			//vertices[i].UV.x = 1 - vertices[i].UV.x;
			//vertices[i].UV.y = 1 - vertices[i].UV.y;
		}

		vertices[i].Normal.x = pMesh->mNormals[i].x;
		vertices[i].Normal.y = pMesh->mNormals[i].y;
		vertices[i].Normal.z = pMesh->mNormals[i].z;

		if (pMesh->HasTangentsAndBitangents())
		{
			vertices[i].Tangent.x = pMesh->mTangents[i].x;
			vertices[i].Tangent.y = pMesh->mTangents[i].y;
			vertices[i].Tangent.z = pMesh->mTangents[i].z;
		}

		if (pMesh->HasVertexColors(0))
		{
			Math::Color color;
			color.x = pMesh->mColors[0][i].r;
			color.y = pMesh->mColors[0][i].g;
			color.z = pMesh->mColors[0][i].b;
			color.w = pMesh->mColors[0][i].a;

			vertices[i].Color = color.RGBA().v;
		}
	}

	faces.resize(pMesh->mNumFaces);

	for (auto i{ Def::UIntZero }; i < pMesh->mNumFaces; ++i)
	{
		faces[i].Idx[0] = pMesh->mFaces[i].mIndices[0];
		faces[i].Idx[1] = pMesh->mFaces[i].mIndices[1];
		faces[i].Idx[2] = pMesh->mFaces[i].mIndices[2];
	}

	std::shared_ptr<Mesh>spMesh = std::make_shared<Mesh>();
	spMesh->Create(&GraphicsDevice::Instance(), vertices, faces, ParseMaterial(pMaterial, dirPath));

	return spMesh;
}

const Material ModelLoader::ParseMaterial(const aiMaterial* pMaterial, const std::string& dirPath)
{
	auto material{ Material {} };

	// �}�e���A���̖��O���擾
	{
		aiString name;

		if (pMaterial->Get(AI_MATKEY_NAME, name) == AI_SUCCESS)
		{
			material.Name = name.C_Str();
		}
	}

	// Diffuse�e�N�X�`���̎擾
	{
		aiString path;

		if (pMaterial->GetTexture(AI_MATKEY_BASE_COLOR_TEXTURE, &path) == AI_SUCCESS)
		{
			auto filePath = std::string(path.C_Str());

			material.spBaseColorTex = std::make_shared<Texture>();

			if (!material.spBaseColorTex->Load(&GraphicsDevice::Instance(), dirPath + filePath))
			{
				assert(0 && "Diffuse�e�N�X�`���̃��[�h�Ɏ��s");
				return Material();
			}
		}
	}

	// DiffuseColor�̎擾
	{
		aiColor4D diffuse;

		if (pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse) == AI_SUCCESS)
		{
			material.BaseColor.x = diffuse.r;
			material.BaseColor.y = diffuse.g;
			material.BaseColor.z = diffuse.b;
			material.BaseColor.w = diffuse.a;
		}
	}

	// MetallicRoughness�e�N�X�`���̎擾
	{
		aiString path;

		if (pMaterial->GetTexture(AI_MATKEY_METALLIC_TEXTURE, &path) == AI_SUCCESS ||
			pMaterial->GetTexture(AI_MATKEY_ROUGHNESS_TEXTURE, &path) == AI_SUCCESS)
		{
			auto filePath = std::string(path.C_Str());

			material.spMetallicRoughnessTex = std::make_shared<Texture>();

			if (!material.spMetallicRoughnessTex->Load(&GraphicsDevice::Instance(), dirPath + filePath))
			{
				assert(0 && "MetallicRoughness�e�N�X�`���̃��[�h�Ɏ��s");
				return Material();
			}
		}
	}

	// Metallic���擾
	{
		auto metallic{ Def::FloatZero };

		if (pMaterial->Get(AI_MATKEY_METALLIC_FACTOR, metallic) == AI_SUCCESS)
		{
			material.Metallic = metallic;
		}
	}

	// Roughness
	{
		auto roughness{ Def::FloatOne };

		if (pMaterial->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness) == AI_SUCCESS)
		{
			material.Roughness = roughness;
		}
	}

	// Emissive�e�N�X�`���̎擾
	{
		auto path{ aiString{} };

		if (pMaterial->GetTexture(AI_MATKEY_EMISSIVE_TEXTURE, &path) == AI_SUCCESS)
		{
			auto filePath = std::string(path.C_Str());

			material.spEmissiveTex = std::make_shared<Texture>();

			if (!material.spEmissiveTex->Load(&GraphicsDevice::Instance(), dirPath + filePath))
			{
				assert(false && "Emissive�e�N�X�`���̃��[�h�Ɏ��s");
				return Material();
			}
		}
	}

	// Emissive�̎擾
	{
		aiColor3D emissive;

		if (pMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, emissive) == AI_SUCCESS)
		{
			material.Emissive.x = emissive.r;
			material.Emissive.y = emissive.g;
			material.Emissive.z = emissive.b;
		}
	}

	// �@���e�N�X�`���̎擾
	{
		aiString path;

		if (pMaterial->GetTexture(AI_MATKEY_NORMAL_TEXTURE, &path) == AI_SUCCESS)
		{
			auto filePath = std::string(path.C_Str());

			material.spNormalTex = std::make_shared<Texture>();

			if (!material.spNormalTex->Load(&GraphicsDevice::Instance(), dirPath + filePath))
			{
				assert(0 && "Normal�e�N�X�`���̃��[�h�Ɏ��s");
				return Material();
			}
		}
	}

	return material;
}
