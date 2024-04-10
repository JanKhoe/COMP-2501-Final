#ifndef ENEMY_GAME_OBJECT_H

#define ENEMY_GAME_OBJECT_H

#include "game_object.h"

namespace game {
	class EnemyGameObject : public GameObject {

	public:
		EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture) :
			GameObject(position, geom, shader, texture) {
			canMove = true;
			type = ENEMY;
			target_ = nullptr;
		}
		inline void die() { canMove = false; }

		GameObject* GetTarget(void) { return target_; }
		void SetTarget(GameObject* t) { target_ = t; }
	protected:
		bool canMove;
		GameObject* target_;

	};
}



#endif