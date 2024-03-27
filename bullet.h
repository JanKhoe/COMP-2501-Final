#ifndef BULLET_H
#define BULLET_H

#include "Timer.h"
#include "game_object.h"

#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>

namespace game {
	class Bullet: public GameObject {
	public:
		Bullet(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);
		void Update(double delta_time) override;
		glm::vec3 dir;
		float speed;
		Timer lifespan;
		glm::vec3 startingPos;
		glm::vec3 prevPosition;

	};
}


#endif // !BULLET_H

