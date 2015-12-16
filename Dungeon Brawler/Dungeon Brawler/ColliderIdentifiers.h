#ifndef COLLIDERIDENTIFIERS_H
#define COLLIDERIDENTIFIERS_H

#include <inttypes.h>

namespace Colliders
{
	enum class ColliderTags : uint16_t
	{
		Ground = 0x0001,
		Player = 0x0002,
		Ai = 0x0004,

		PlayerProjectile = 0x0008,
		EnemyProjectile = 0x0010,
		TrapProjectile = 0x0020,

		PlayerMelee = 0x0040,
		EnemyMelee = 0x0080,
		TrapMelee = 0x0100,
	};

	enum class ColliderMasks : uint16_t
	{
		PlayerMask = (uint16_t)ColliderTags::Ground | (uint16_t)ColliderTags::Ai | (uint16_t)ColliderTags::EnemyProjectile | (uint16_t)ColliderTags::TrapProjectile | (uint16_t)ColliderTags::EnemyMelee | (uint16_t)ColliderTags::TrapMelee,
		AIMask = (uint16_t)ColliderTags::Ground | (uint16_t)ColliderTags::Ai | (uint16_t)ColliderTags::PlayerProjectile | (uint16_t)ColliderTags::TrapProjectile | (uint16_t)ColliderTags::Player | (uint16_t)ColliderTags::PlayerMelee | (uint16_t)ColliderTags::TrapMelee,
		TrapMask = (uint16_t)ColliderTags::Ground | (uint16_t)ColliderTags::Ai | (uint16_t)ColliderTags::EnemyProjectile | (uint16_t)ColliderTags::PlayerProjectile | (uint16_t)ColliderTags::Player | (uint16_t)ColliderTags::PlayerMelee | (uint16_t)ColliderTags::EnemyMelee,
		
		GroundOnly = (uint16_t)ColliderTags::Ground,

		PlayerMeleeMask = (uint16_t)ColliderTags::Ai | (uint16_t)ColliderTags::EnemyProjectile | (uint16_t)ColliderTags::TrapProjectile,
		AIMeleeMask = (uint16_t)ColliderTags::PlayerProjectile | (uint16_t)ColliderTags::TrapProjectile | (uint16_t)ColliderTags::Player,
		TrapMeleeMask = (uint16_t)ColliderTags::Ai | (uint16_t)ColliderTags::EnemyProjectile | (uint16_t)ColliderTags::PlayerProjectile | (uint16_t)ColliderTags::Player,
	};
}


#endif //COLLIDERIDENTIFIERS_H