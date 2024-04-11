#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public GameObject {

        

        public:

            PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);

            // Update function for moving the player object around
            void Update(double delta_time) override;

            void Render(glm::mat4 view_matrix, double current_time) override;

            int health;

            void SuperActive();

            bool isSuperActive;

            void incrementCoinCount();
            
            bool isInvincible;
        protected:
            
            int coins_collected;
            Timer SuperTime;
            Timer iFrames;
            

    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_
