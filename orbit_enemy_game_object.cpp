#include "orbit_enemy_game_object.h"
#include <glm/gtc/matrix_transform.hpp> 

namespace game {
	Orbit_EnemyGameObject::Orbit_EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: EnemyGameObject(position, geom, shader, texture) {
		speed = 3.0f;
		orbitRadius = 3.0f;
		current_time = rand() % 6;
		canDestroyItems = true;
	}


	void Orbit_EnemyGameObject::Update(double delta_time) {
		if (target_ == nullptr) {
			return;
		}

		current_time += delta_time;
		float xPos = orbitRadius * glm::cos(current_time * speed) + target_->GetPosition().x;
		float yPos = orbitRadius * glm::sin(current_time * speed) + target_->GetPosition().y;


		angle_ = (current_time*speed) + glm::pi<float>() / 2;

		position_ = glm::vec3(xPos, yPos, 0);

	}
}