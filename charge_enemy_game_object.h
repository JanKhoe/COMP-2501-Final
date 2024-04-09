#ifndef CHARGE_ENEMY_GAME_OBJECT_H
#define CHARGE_ENEMY_GAME_OBJECT_H

#include "enemy_game_object.h"
#include "Timer.h"

namespace game {
	class Charge_EnemyGameObject : public EnemyGameObject {

		enum states {PATROL, READY, LAUNCH};

	public:
		Charge_EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);
		void Update(double delta_time) override;

		// Update target
		GameObject* GetTarget(void) { return target_; }
		void SetTarget(GameObject* t) { target_ = t; }

		// New Velocity Variable
		inline glm::vec3 GetVelocity(void) const { return velocity_; }
		inline void SetVelocity(glm::vec3 v) { velocity_ = v; }

		states state;

	protected:


		glm::vec3 velocity_;
		glm::vec3 startingPos;
		glm::vec3 targetPos;
		glm::vec3 orbitPos;

		float seekRadius;
		float orbitRadius;
		float charge_up_speed;
		float launch_speed;

		double current_time;
		GameObject* target_;

		void PatrolInput(double);
		void ReadyInput(double);
		void LaunchInput(double);

		Timer windUp;
		
	};
}




#endif
