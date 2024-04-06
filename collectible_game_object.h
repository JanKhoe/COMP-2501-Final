#ifndef COLLECTIBLE_GAME_OBJECT_H_
#define COLLECTIBLE_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    enum collectType { ORANGESODA, JUICERJUICE, NOOBMAXXER };

    // Inherits from GameObject
    class CollectibleGameObject : public GameObject {

        public:
            CollectibleGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);

            // Getter
            inline collectType GetCollectType() const { return collectType; }
            // Setter
            inline void SetCollectType(collectType type) { collectType = type; }

        private:
            collectType collectType;

    }; // class CollectibleGameObject

} // namespace game

#endif // COLLECTIBLE_GAME_OBJECT_H_
