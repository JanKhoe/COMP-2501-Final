#ifndef PATROL_CHASE_GAME_OBJECT_H
#define PATROL_CHASE_GAME_OBJECT_H

#include "enemy_game_object.h"


namespace game {
	class Patrol_Chase_EnemyGameObject : public EnemyGameObject{

        enum states {PATROL, CHASE};
	
    public:
        Patrol_Chase_EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

        // Update function for moving the player object around
        void Update(double delta_time) override;

        // Update target
        GameObject* GetTarget(void) { return target_; }
        void SetTarget(GameObject* t) { target_ = t; }

        // New Velocity Variable
        inline glm::vec3 GetVelocity(void) const { return velocity_; }
        inline void SetVelocity(glm::vec3 v) { velocity_ = v; }

    protected:
        GameObject* target_;
        double wander_cool_down_;
        double current_time_;
        glm::vec3 velocity_;
        
        states state_;

        void PatrolInput(double delta_time);
        void ChaseInput(double delta_time);
	};
}

#endif // !PATROL_CHASE_GAME_OBJECT_H
