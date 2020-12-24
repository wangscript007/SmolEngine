#pragma once
#include "Core/Core.h"

#include <cereal/cereal.hpp>
#include <cereal/types/unordered_map.hpp>

namespace SmolEngine
{
	struct AnimationClip;

	struct Animation2DComponent
	{
		Animation2DComponent();

		///
		
		std::unordered_map<std::string, Ref<AnimationClip>> m_Clips;

		/// 
		
		int IndexLayer = 0;

		///

		Ref<AnimationClip> CurrentClip = nullptr;

	private:

		friend class EditorLayer;

		friend class WorldAdmin;

		friend class cereal::access;

		template<typename Archive>
		void serialize(Archive & archive)
		{
			archive(m_Clips, CurrentClip, IndexLayer);
		}
	};
}