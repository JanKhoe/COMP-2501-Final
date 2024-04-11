#ifndef SUPER_COLLECTIBLE_GAME_OBJECT_H

#define  SUPER_COLLECTIBLE_GAME_OBJECT_H

#include "collectible_game_object.h"

namespace game {
	class SuperCollectibleGameObject : public CollectibleGameObject {
	public:
		SuperCollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

	};
}



#endif