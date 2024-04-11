#ifndef ORBIT_ENEMY_GAME_OBJECT_H
#define ORBIT_ENEMY_GAME_OBJECT_H

#include "enemy_game_object.h"

namespace game {
	class Orbit_EnemyGameObject : public EnemyGameObject {
	public:
		Orbit_EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

		void Update(double delta_time) override;
	protected:
		float speed;
		float orbitRadius;
		double current_time;

	};
}



#endif // !ORBIT_ENEMY_GAME_OBJECT_H

