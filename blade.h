#ifndef BLADE_H_
#define BLADE_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class Blade : public GameObject {

    public:
        Blade(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* parent);

        // Update function for moving the player object around
        void Update(double delta_time) override;

        void Render(glm::mat4 view_matrix, double current_time) override;

        GameObject* parent_;

    }; // class PlayerGameObject

} // namespace game

#endif // BLADE_H_
