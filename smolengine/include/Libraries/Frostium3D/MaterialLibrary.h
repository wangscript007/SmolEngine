#pragma once
#include "Common/Core.h"
#include "Common/Texture.h"

#include "Utils/GLM.h"

#include <string>
#include <optional>
#include <mutex>
#include <cereal/cereal.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>

namespace Frostium
{
	enum class MaterialTexture : uint32_t
	{
		Albedro,
		Normal,
		Metallic,
		Roughness,
		AO
	};

	struct PBRMaterial
	{
		alignas(16) glm::vec4 PBRValues; // x - Metallic, y - Roughness, z - Albedro

		alignas(4) uint32_t UseAlbedroTex;
		alignas(4) uint32_t UseNormalTex;
		alignas(4) uint32_t UseMetallicTex;
		alignas(4) uint32_t UseRoughnessTex;
		alignas(4) uint32_t UseAOTex;

		alignas(4) uint32_t AlbedroTexIndex;
		alignas(4) uint32_t NormalTexIndex;
		alignas(4) uint32_t MetallicTexIndex;
		alignas(4) uint32_t RoughnessTexIndex;
		alignas(4) uint32_t AOTexIndex;

		alignas(4) uint32_t pad1;
		alignas(4) uint32_t pad2;
	};

	struct MaterialCreateInfo
	{
		// Setters
		void SetMetalness(float value);
		void SetRoughness(float value);
		void SetAlbedro(float value);
		void SetTexture(MaterialTexture type, const std::string& filePath);

		// Getters
		float* GetMetalness();
		float* GetRoughness();
		std::unordered_map<MaterialTexture, std::string>& GetTexturesInfo();

	private:

		float                                             Metallic = 1.0f;
		float                                             Albedro = 1.0f;
		float                                             Roughness = 1.0f;
		bool                                              Used = false;
					                                      
		std::unordered_map<MaterialTexture, std::string>  Textures;

	private:

		friend class MaterialLibrary;
		friend class cereal::access;

		template<typename Archive>
		void serialize(Archive& archive)
		{
			archive(Metallic, Albedro, Roughness, Textures);
		}
	};

	class MaterialLibrary
	{
	public:

		MaterialLibrary();
		~MaterialLibrary();

		uint32_t Add(MaterialCreateInfo* infoCI, const std::string& path = "");
		bool Delete(const std::string& name);
		void Reset();

		// Serialization
		bool Load(const std::string& filePath, MaterialCreateInfo& out_info);
		bool Save(const std::string& filePath, MaterialCreateInfo& info);

		// Getters
		static MaterialLibrary* GetSinglenton();
		PBRMaterial* GetMaterial(uint32_t ID);
		PBRMaterial* GetMaterial(std::string& path);
		int32_t GetMaterialID(std::string& path);
		int32_t GetMaterialID(size_t& hashed_path);
		std::vector<PBRMaterial>& GetMaterials();
		void GetMaterialsPtr(void*& data, uint32_t& size);
		void GetTextures(std::vector<Texture*>& out_textures) const;

	private:

		// Helpers
		uint32_t AddTexture(const std::string& path, uint32_t& useTetxure);
		std::string GetCompleteName(MaterialCreateInfo* infoCI);

	private:

		static MaterialLibrary*               s_Instance;
		uint32_t                              m_MaterialIndex = 0;
		uint32_t                              m_TextureIndex = 0;
#ifdef FROSTIUM_SMOLENGINE_IMPL
		std::mutex                            m_Mutex{};
#endif
		std::vector<PBRMaterial>              m_Materials;
		std::vector<Texture*>                 m_Textures;
		std::hash<std::string_view>           m_Hash{};
		std::unordered_map<size_t, uint32_t>  m_MaterialMap;
	};
}