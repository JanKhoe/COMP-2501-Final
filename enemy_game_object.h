#ifndef ENEMY_GAME_OBJECT_H_
#define ENEMY_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class EnemyGameObject : public GameObject {

        public:
            EnemyGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);

            // Update function for moving the player object around
            void Update(double delta_time) override;

            // Update target
            GameObject *GetTarget(void) { return target_; }
            void SetTarget(GameObject *t) { target_ = t; }

            // New Velocity Variable
            inline glm::vec3 GetVelocity(void) const { return velocity_; }
            inline void SetVelocity(glm::vec3 v) { velocity_ = v; }

        protected:
            GameObject *target_;
            double wander_cool_down_;
            double current_time_;
            glm::vec3 velocity_;

    }; // class EnemyGameObject

} // namespace game

#endif // ENEMY_GAME_OBJECT_H_
