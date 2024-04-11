

#ifndef INVINCIBLE_COLLECTIBLE_GAME_OBJECT_H
#define INVINCIBLE_COLLECTIBLE_GAME_OBJECT_H

#include "collectible_game_object.h"

namespace game {
	class InvincibleCollectibleGameObject : public CollectibleGameObject {
	public:
		InvincibleCollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

	};
}

#endif // !INVINCIBLE_COLLECTIBLE_GAME_OBJECT_H

