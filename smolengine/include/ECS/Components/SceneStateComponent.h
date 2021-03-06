#pragma once

#include "ECS/Actor.h"

#include <string>
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>
#include <cereal/cereal.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/unordered_map.hpp>

namespace SmolEngine
{
	struct SceneStateComponent
	{
		uint32_t                                       SceneID = 0;
		uint32_t                                       LastActorID = 0;
		std::string                                    FilePath = "";
		std::string                                    Name = std::string("");
		std::unordered_map<std::string, Actor*>        ActorNameSet;
		std::unordered_map<uint32_t, Actor*>           ActorIDSet;
		std::vector<Actor>                             Actors;

	public:

		SceneStateComponent() = default;

	private:

		friend class cereal::access;
		template<typename Archive>
		void serialize(Archive& archive)
		{
			archive(SceneID, LastActorID, FilePath, Name, Actors);
		}
	};
}