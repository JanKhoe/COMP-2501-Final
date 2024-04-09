#ifndef ENEMY_GAME_OBJECT_H

#define ENEMY_GAME_OBJECT_H

#include "game_object.h"

namespace game {
	class EnemyGameObject : public GameObject {

	public:
		EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture) :
			GameObject(position, geom, shader, texture) {}

	};
}



#endif