#include "blade.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

namespace game {

	/*
		PlayerGameObject inherits from GameObject
		It overrides GameObject's update method, so that you can check for input to change the velocity of the player
	*/

	Blade::Blade(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* parent)
		: GameObject(position, geom, shader, texture) {
        parent_ = parent;
    }

	// Update function for moving the player object around
	void Blade::Update(double delta_time) {
		SetRotation(GetRotation() + delta_time);
		// Special player updates go here

		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);
	}

	void Blade::Render(glm::mat4 view_matrix, double current_time) {

        shader_->Enable();
        shader_->SetUniformMat4("view_matrix", view_matrix);


        std::vector<glm::mat4> parent_transformations;
        GameObject* curParent = nullptr;
        if (parent_ != nullptr) {
            curParent = parent_;
        }

        while (curParent != nullptr) {
            glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(curParent->GetScale(), curParent->GetScale(), 1.0));

            // Setup the rotation matrix for the shader
            glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), curParent->GetRotation(), glm::vec3(0.0, 0.0, 1.0));

            // Set up the translation matrix for the shader
            glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), curParent->GetPosition());

            glm::mat4 parent_transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;

            parent_transformations.push_back(parent_transformation_matrix);
            curParent = curParent->GetParent();
        }
        // Set up the shader
        

        // Set up the view matrix
        

        // Setup the scaling matrix for the shader
        glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_, scale_, 1.0));

        // Setup the rotation matrix for the shader
        glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));

        // Set up the translation matrix for the shader
        glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);

        // Setup the transformation matrix for the shader
        glm::mat4  transformation_matrix;
        
        
        for (int i = parent_transformations.size()-1; i >=0; i--) {
            transformation_matrix *= parent_transformations[i];
            //std::cout << "THis is from render blade" << i << std::endl;
        }
        
        transformation_matrix *= (translation_matrix * rotation_matrix * scaling_matrix);

        //transformation_matrix = (translation_matrix * rotation_matrix * scaling_matrix);

        // Set the transformation matrix in the shader
        shader_->SetUniformMat4("transformation_matrix", transformation_matrix);

        // Set up the geometry
        geometry_->SetGeometry(shader_->GetShaderProgram());

        // Bind the entity's texture
        glBindTexture(GL_TEXTURE_2D, texture_);

        // Draw the entity
        glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
		
	}

} // namespace game
