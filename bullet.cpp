#include "bullet.h"
#include <iostream>


namespace game {
	using namespace std;
	Bullet::Bullet(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: GameObject(position, geom, shader, texture) {
		lifespan = Timer(0.5f);
		speed = 25.0f;
		scale_ = 0.3;
		startingPos = position;
		prevPosition = position;
	}

	void Bullet::Update(double delta_time) {
		prevPosition = position_;
		SetPosition(position_ + (-GetRight() * speed) * (float)delta_time);
		//cout << "new pos: " << position_.x<<position_.y << endl;
	}
}