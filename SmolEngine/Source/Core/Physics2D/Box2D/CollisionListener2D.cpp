#include "stdafx.h"
#include "CollisionListener2D.h"
#include "Core/ECS/Actor.h"

namespace SmolEngine
{
	void CollisionListener2D::BeginContact(b2Contact* contact)
	{
		void* dataA = contact->GetFixtureA()->GetBody()->GetUserData();
		void* dataB = contact->GetFixtureB()->GetBody()->GetUserData();

		const auto actorB = static_cast<Actor*>(dataB);
		if (IsValid(actorB))
		{
			const auto actorA = static_cast<Actor*>(dataA);
			//auto& script = actorB->GetComponent<ScriptObject>();
			//if (contact->GetFixtureA()->IsSensor())
			//{
			//	script.OnTriggerContact(actorA);
			//	return;
			//}

			//script.OnCollisionContact(actorA);
		}

	}

	void CollisionListener2D::EndContact(b2Contact* contact)
	{
		void* dataA = contact->GetFixtureA()->GetBody()->GetUserData();
		void* dataB = contact->GetFixtureB()->GetBody()->GetUserData();

		const auto actorB = static_cast<Actor*>(dataB);
		if (IsValid(actorB))
		{
			const auto actorA = static_cast<Actor*>(dataA);
			//auto& script = actorB->GetComponent<ScriptObject>();
			//if (contact->GetFixtureA()->IsSensor())
			//{
			//	script.OnTriggerExit(actorA);
			//	return;
			//}

			//script.OnCollisionExit(actorA);
		}
	}

	void CollisionListener2D::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{

	}

	void CollisionListener2D::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{

	}

	bool CollisionListener2D::IsValid(Actor* actor) const
	{
		return false;
	}
}