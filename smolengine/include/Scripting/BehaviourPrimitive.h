#pragma once

namespace SmolEngine
{
	class Actor;
	class BehaviourPrimitive
	{
	public:

		BehaviourPrimitive() = default;
		virtual ~BehaviourPrimitive() = default;

		// Getters
		const std::string& GetName();
		const std::string& GetTag();
		const size_t GetID();
		void GetActors(std::vector<Actor*>& outList);
		void GetActorsWithTag(const std::string& tag, std::vector<Actor*>& outList);

		// Search
		Actor* FindActorByName(const std::string& name);
		Actor* FindActorByTag(const std::string& tag);
		Actor* FindActorByID(uint32_t id);

		template<typename T>
		bool CreateValue(const std::string& name)
		{
			OutValue info = {};
			info.Ptr = nullptr;
			info.ValueName = name;

			if (std::is_same<int32_t, T>::value)
				info.Type = OutValueType::Int;

			if (std::is_same<float, T>::value)
				info.Type = OutValueType::Float;

			if (std::is_same<std::string, T>::value)
				info.Type = OutValueType::String;

			if (info.Type != OutValueType::None)
			{
				m_OutValues.emplace_back(info);
				return true;
			}

			return false;
		}

		template<typename T>
		T* GetValue(const std::string& name)
		{
			for (const auto& value : m_OutValues)
			{
				if (value.ValueName == name)
					return static_cast<T*>(value.Ptr);
			}

			return nullptr;
		}

		template<typename T>
		T* GetComponent() { return WorldAdmin::GetSingleton()->GetActiveScene().GetComponent<T>(m_Actor); }

		template<typename T>
		T* HasComponent() { return WorldAdmin::GetSingleton()->GetActiveScene().HasComponent<T>(m_Actor); }

	private:

		enum class OutValueType : uint16_t
		{
			None,
			Int,
			Float,
			String
		};

		struct OutValue
		{
			void*        Ptr = nullptr;
			std::string  ValueName = "";
			OutValueType Type = OutValueType::None;
		};

		Actor*                   m_Actor = nullptr;
		std::vector<OutValue>    m_OutValues;

	private:

		friend class SystemRegistry;
		friend class CollisionListener2D;
		friend class ScriptingSystem;
		friend class WorldAdmin;
		friend class Scene;
	};
}