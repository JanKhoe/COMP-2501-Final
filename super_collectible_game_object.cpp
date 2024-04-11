#include "super_collectible_game_object.h"


namespace game {
	SuperCollectibleGameObject::SuperCollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture) :
		CollectibleGameObject(position, geom, shader, texture) {
		collectType = JUICERJUICE;
	}
}