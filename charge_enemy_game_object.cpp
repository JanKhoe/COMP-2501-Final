#include "charge_enemy_game_object.h"
#include <iostream>
using namespace std;


namespace game {

	Charge_EnemyGameObject::Charge_EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture) :
		EnemyGameObject(position, geom, shader, texture) {
		state = PATROL;
		target_ = nullptr;
		velocity_ = glm::vec3(0, 0, 0);
		orbitPos = position;
		orbitRadius = 2.0f;
		seekRadius = 2;
		charge_up_speed = 1;
		launch_speed = 10.0f;
		current_time = 0;
	
	}


	//Parabolic function to have an ease-in-ease-out transition for charging steering force
	float map(float input) {
		return ( -2 * pow(input - 0.5, 2)) + 0.5;
	}

	void Charge_EnemyGameObject::Update(double delta_time) {
		current_time += delta_time;
		angle_ = glm::atan(velocity_.y, velocity_.x);


		//position_ = glm::vec3(1, 2, 0);
		//return;
		if (target_ == nullptr) {
			return;
		}
		
		//switch (state)
		//{
		//case game::Charge_EnemyGameObject::PATROL:
		//	PatrolInput(delta_time);
//
		//case game::Charge_EnemyGameObject::READY:
		//	ReadyInput(delta_time);
		//case game::Charge_EnemyGameObject::LAUNCH:
		//	LaunchInput(delta_time);
		
		//}

		if (state == PATROL) {
			PatrolInput(delta_time);
		}
		else if (state == READY) {
			ReadyInput(delta_time);
		}
		else if (state == LAUNCH) {
			LaunchInput(delta_time);
		}


		GameObject::Update(delta_time);
	}


	void Charge_EnemyGameObject::PatrolInput(double delta_time) {
		
		//cout << orbitPos.x << glm::cos(current_time) << endl;
		//cout << "waht" << position_.x << position_.y << endl;
		
		float xPos = glm::cos(current_time * orbitRadius) + orbitPos.x;
		float yPos = glm::sin(current_time * orbitRadius) + orbitPos.y;

		position_ = glm::vec3(xPos, yPos, 0.0f);
		//cout << "waht" << position_.x << position_.y << endl;

		if (abs(glm::length(position_ - target_->GetPosition())) < seekRadius) {
			startingPos = position_;
			state = Charge_EnemyGameObject::READY;
			windUp = Timer(1.5f);
			cout << "Charging Up!" << endl;
		}
		return;
	}

	void Charge_EnemyGameObject::ReadyInput(double delta_time) {
		//A - B = vector from B to A
		glm::vec3 desired = glm::normalize(startingPos - target_->GetPosition());
		position_ += desired * charge_up_speed * (float)delta_time;
		if (windUp.Finished()) {
			state = LAUNCH;
			startingPos = position_;
			targetPos = target_->GetPosition();
			cout << "LAUNCHING" << endl;
		}

	}

	void Charge_EnemyGameObject::LaunchInput(double delta_time) {
		glm::vec3 desired = (targetPos - startingPos) + glm::normalize(targetPos - startingPos)/2.0f;
		//float progress = glm::length((position_ - startingPos) - (desired - startingPos));
		float progress = (glm::length(position_ - startingPos) / glm::length(desired - startingPos)) + 0.1f;
		//cout << progress << endl;
		if (abs(progress) > 0.99f) {
			state = PATROL;
			orbitPos = glm::vec3(position_.x - orbitRadius, position_.y + orbitRadius, 0);
			current_time = 0;
			cout << "Finished Launch" << endl;
		}
		position_ += glm::normalize(desired - startingPos) * launch_speed * map(progress) * (float)delta_time;
	}


} 