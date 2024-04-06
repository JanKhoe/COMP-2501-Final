#include <stdexcept>
#include <string>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> 
#include <SOIL/SOIL.h>
#include <iostream>

#include <path_config.h>

#include "sprite.h"
#include "particles.h"
#include "shader.h"
#include "player_game_object.h"
#include "enemy_game_object.h"
#include "collectible_game_object.h"
#include "particle_system.h"
#include "bullet.h"
#include "game.h"
#include "blade.h"


namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Globals that define the OpenGL window and viewport
const char *window_title_g = "Game Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const glm::vec3 viewport_background_color_g(0.0, 0.0, 1.0);

// Directory with game resources such as textures
const std::string resources_directory_g = RESOURCES_DIRECTORY;


Game::Game(void)
{
    // Don't do work in the constructor, leave it for the Init() function
}


void Game::Init(void)
{
    fireRate = Timer(0);

    std::srand((unsigned)time(0));
    // Initialize the window management library (GLFW)
    if (!glfwInit()) {
        throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
    }

    // Set whether window can be resized
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); 

    // Create a window and its OpenGL context
    window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g, NULL, NULL);
    if (!window_) {
        glfwTerminate();
        throw(std::runtime_error(std::string("Could not create window")));
    }

    // Make the window's OpenGL context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
    }

    // Set event callbacks
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    // Initialize sprite geometry
    sprite_ = new Sprite();
    sprite_->CreateGeometry();

    // Initialize particle geometry
    particles_ = new Particles();
    particles_->CreateGeometry();

    bullettrail_ = new Particles();
    bullettrail_->SetScope(1.0f);

    explosiontrail_ = new Particles();
    explosiontrail_->SetScope(64.0f);
    //bullettrail_->CreateGeometry();


    // Initialize particle shader
    particle_shader_.Init((resources_directory_g+std::string("/particle_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/particle_fragment_shader.glsl")).c_str());

    // Initialize sprite shader
    sprite_shader_.Init((resources_directory_g+std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/sprite_fragment_shader.glsl")).c_str());

    // Initialize time
    current_time_ = 0.0;
    accel = 0.0005f;
    friction = 0.0025f;
    HorVelocity = 0.0f;
    VerVelocity = 0.0f;
    canMove = true;
    recoilForce = 0.005f;

    globalVel = glm::vec3(0.0f, 0.0f, 0.0f);
}


Game::~Game()
{
    // Free memory for all objects
    // Only need to delete objects that are not automatically freed
    delete sprite_;
    delete particles_;
    for (int i = 0; i < game_objects_.size(); i++){
        delete game_objects_[i];
    }

    for (int i = 0; i < bullets_.size(); i++) {
        delete bullets_[i];
    }

    // Close window
    glfwDestroyWindow(window_);
    glfwTerminate();
}


void Game::Setup(void)
{

    // Setup the game world

    // Load textures
    SetAllTextures();

    playerContainer = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[7]);

    // Setup the player object (position, texture, vertex count)
    // Note that, in this specific implementation, the player object should always be the first object in the game object vector 
    game_objects_.push_back(new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[0]));
    float pi_over_two = glm::pi<float>() / 2.0f;

    // Setup other objects
    // Test Enemy AI Object
    EnemyGameObject *newEnemy = new EnemyGameObject(glm::vec3(1.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[2]);
    newEnemy->SetRotation(pi_over_two);
    newEnemy->type = ENEMY;
    newEnemy->SetTarget(playerContainer);
    newEnemy->SetVelocity(glm::vec3(0.0, 0.5, 0.0));
    game_objects_.push_back(newEnemy);

    game_objects_.push_back(new GameObject(glm::vec3(1.05f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[2]));
    game_objects_[2]->SetRotation(pi_over_two);
    game_objects_[2]->type = ENEMY;
    game_objects_.push_back(new GameObject(glm::vec3(rand() % 5, rand() % 5, 0.0f), sprite_, &sprite_shader_, tex_[2]));
    game_objects_[3]->SetRotation(pi_over_two);
    game_objects_[3]->type = game::ENEMY;
    game_objects_.push_back(new GameObject(glm::vec3(rand() % 5, rand() % 5, 0.0f), sprite_, &sprite_shader_, tex_[2]));
    game_objects_[4]->SetRotation(pi_over_two);
    game_objects_[4]->type = game::ENEMY;
    game_objects_.push_back(new GameObject(glm::vec3(rand() % 5, rand() % 5, 0.0f), sprite_, &sprite_shader_, tex_[2]));
    game_objects_[5]->SetRotation(pi_over_two);
    game_objects_[5]->type = game::ENEMY;

    // Setup Collectibles
    game_objects_.push_back(new CollectibleGameObject(glm::vec3(-2.0f, -1.0f, 0.0f), sprite_, &sprite_shader_, tex_[9]));
    //game_objects_[5]->type = game::COLLECTIBLE; < Does it already in the constructor

    blade = new Blade(glm::vec3(0, 0, 0.0f), sprite_, &sprite_shader_, tex_[6], game_objects_[0]);
    //game_objects_.push_back(new GameObject(glm::vec3(rand()%5, rand()%5, 0.0f), sprite_, &sprite_shader_, tex_[2]));
    //game_objects_[3]->SetRotation(pi_over_two);

    // Setup background
    // In this specific implementation, the background is always the
    // last object
    GameObject *background = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[3]);
    background->SetScale(20.0);
    game_objects_.push_back(background);

    // Setup particle system
    GameObject *particles = new ParticleSystem(glm::vec3(-0.5f, 0.0f, 0.0f), particles_, &particle_shader_, tex_[4], game_objects_[0]);
    particles->SetScale(0.2);
    particles->SetRotation(-pi_over_two);
    game_objects_.push_back(particles);

    player = dynamic_cast<PlayerGameObject*>(game_objects_[0]);
    player->SetParent(playerContainer);


}


void Game::ResizeCallback(GLFWwindow* window, int width, int height)
{

    // Set OpenGL viewport based on framebuffer width and height
    glViewport(0, 0, width, height);
}


void Game::SetTexture(GLuint w, const char *fname)
{
    // Bind texture buffer
    glBindTexture(GL_TEXTURE_2D, w);

    // Load texture from a file to the buffer
    int width, height;
    unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image){
        std::cout << "Cannot load texture " << fname << std::endl;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    // Texture Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Texture Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void Game::SetAllTextures(void)
{
    // Load all textures that we will need
    // Declare all the textures here
    const char *texture[] = {"/textures/playerPlane.png", "/textures/destroyer_green.png", "/textures/spiker.png", "/textures/stars2.png", "/textures/orb.png", "/textures/bullet.png", "/textures/blade.png", "/textures/Empty.png", "/textures/explosion.png", "/textures/orange_soda.png"};
    // Get number of declared textures
    int num_textures = sizeof(texture) / sizeof(char *);
    // Allocate a buffer for all texture references
    tex_ = new GLuint[num_textures];
    glGenTextures(num_textures, tex_);
    // Load each texture
    for (int i = 0; i < num_textures; i++){
        SetTexture(tex_[i], (resources_directory_g+std::string(texture[i])).c_str());
    }
    // Set first texture in the array as default
    glBindTexture(GL_TEXTURE_2D, tex_[0]);
}


void Game::MainLoop(void)
{
    // Loop while the user did not close the window
    double last_time = glfwGetTime();
    while (!glfwWindowShouldClose(window_)){

        // Calculate delta time
        double current_time = glfwGetTime();
        double delta_time = current_time - last_time;
        last_time = current_time;

        // Update window events like input handling
        glfwPollEvents();

        // Handle user input
        HandleControls(delta_time);

        // Update all the game objects
        Update(delta_time);

        // Render all the game objects
        Render();

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);
    }
}


void Game::HandleControls(double delta_time)
{
    if (!canMove) {
        return;
    }
    //applied_force_hor = false;
    applied_force_ver = false;
    // Get player game object
    
    // Get current position and angle
    glm::vec3 curpos = playerContainer->GetPosition();
    float angle = player->GetRotation();
    // Compute current bearing direction
    glm::vec3 dir = player->GetBearing();
    // Adjust motion increment and angle increment 
    // if translation or rotation is too slow
    float speed = delta_time * 1600.0;
    float motion_increment = 0.001 * speed;
    float angle_increment = (glm::pi<float>() / 1800.0f) * speed;

    double xpos, ypos;
    int windowSizeX, windowSizeY;
    glfwGetWindowSize(window_, &windowSizeX, &windowSizeY);
    glfwGetCursorPos(window_, &xpos, &ypos);
    glm::vec3 mouse_pos = glm::vec3((xpos - (windowSizeX / 2)), (ypos - (windowSizeY / 2)), 0.0);

    player->SetRotation(glm::atan(player->GetPosition().x - mouse_pos.x, player->GetPosition().y - mouse_pos.y) + glm::pi<float>() / 2);

    //std::cout << "Mouse Position: " << xpos << ", " << ypos << std::endl;
    //std::cout << "Mouse Position: " << xpos << ", " << ypos << std::endl;

    // Check for player input and make changes accordingly
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
        //player->SetPosition(curpos + motion_increment*dir);
        VerVelocity = 0.002;
        //Since a game runs in approximately 60fps that means there are 60 update calls in a second?
        // So if i want the max speed to be 2units/s then i need to cap the speed that is incremented 
        // In the game engine to 2/60.  
        applied_force_ver = true;
    }

    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
        //player->SetPosition(curpos - motion_increment * dir);
        VerVelocity = -0.002;
        applied_force_ver = true;
    }
    if (glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (fireRate.Finished()) {
            bullets_.push_back(new Bullet(playerContainer->GetPosition(), sprite_, &sprite_shader_, tex_[5]));
            //bullets_[bullets_.size() - 1]->SetRotation(player->GetRotation());
            bullets_[bullets_.size() - 1]->SetRotation(player->GetRotation() - glm::pi<float>() / 2.0f);
            fireRate.Start(1);
            bullettrails_.push_back(new ParticleSystem(glm::vec3(0.0f, 0.0f, 0.0f), bullettrail_, &particle_shader_, tex_[4], bullets_[bullets_.size() - 1]));
            bullettrails_[bullettrails_.size() - 1]->SetScale(0.1f);
            globalVel += (-1.0f * player->GetBearing() * recoilForce);
        }

    }

    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, true);
    }

    //glm::vec3 HorMovement = HorVelocity * player->GetRight();
    glm::vec3 VerMovement = VerVelocity * dir * (float)delta_time;
    globalVel += VerMovement;
    if (!applied_force_ver) {
        if (globalVel.x != 0) {
            if (globalVel.x > 0) {
                globalVel.x -= friction * delta_time;
                if (globalVel.x < 0.0f) {
                    globalVel.x = 0.0f;
                }
            }
            else if (globalVel.x < 0) {
                globalVel.x += friction * delta_time;
                if (globalVel.x > 0.0f) {
                    globalVel.x = 0.0f;
                }
            }

        }
        if (globalVel.y != 0) {
            if (globalVel.y > 0) {
                globalVel.y -= friction * delta_time;
                if (globalVel.y < 0.0f) {
                    globalVel.y = 0.0f;
                }
            }
            else if (globalVel.y < 0) {
                globalVel.y += friction * delta_time;
                if (globalVel.y > 0.0f) {
                    globalVel.y = 0.0f;
                }
            }

        }
    }
    
    
    //player->SetPosition(curpos + (HorMovement + VerMovement));
    playerContainer->SetPosition(curpos + (globalVel));
    std::cout << playerContainer->GetPosition().x << playerContainer->GetPosition().y << std::endl;
}


void Game::Update(double delta_time)
{

    // Update time
    current_time_ += delta_time;


    
    
    // Update all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
        // Get the current game object
        GameObject* current_game_object = game_objects_[i];

        // Update the current game object
        current_game_object->Update(delta_time);
        if (current_game_object->lifespan.Finished()) {
            
            
            if (i == 0) {
                
                std::cout << "Game Over" << std::endl;
                blade->SetParent(nullptr);
                glfwSetWindowShouldClose(window_, true);
                return;
            }
            delete current_game_object;
            game_objects_.erase(game_objects_.begin() + i);
            i--;
            
        }

        blade->Update(delta_time);

        // Check for collision with other game objects
        // Note the loop bounds: we avoid testing the last object since
        // it's the background covering the whole game world
        //for (int j = i + 1; j < (game_objects_.size()-1); j++) {
        //    GameObject* other_game_object = game_objects_[j];

            // Compute distance between object i and object j
        //    float distance = glm::length(current_game_object->GetPosition() - other_game_object->GetPosition());
            // If distance is below a threshold, we have a collision
          //  if (distance < 0.8f) {
                // This is where you would perform collision response between objects

            //}
        //}
    }

    for (int i = 1; i < game_objects_.size() - 1; i++) {
        GameObject* other_game_object = game_objects_[i];
        float distance = glm::length(playerContainer->GetPosition() - other_game_object->GetPosition());
        if (distance < 0.8f) {
            if (other_game_object->type == ENEMY) {
                other_game_object->lifespan = Timer(5);
                other_game_object->SetTexture(tex_[8]);
                other_game_object->type = EXPLOSION;
                if (--player->health <= 0) {
                    canMove = false;
                    player->SetTexture(tex_[8]);
                    player->lifespan = Timer(5);
                }
            }
            
        }
    }

    for (int i = 0; i < explosion_origins_.size(); i++) {
        if (explosion_origins_[i]->lifespan.Finished()) {
            delete explosion_origins_[i];
            explosion_origins_.erase(explosion_origins_.begin() + i);
            delete explosions_[i];
            explosions_.erase(explosions_.begin() + i);
        }
    }



    for (int i = 0; i < bullets_.size(); i++) {
        if (bullets_[i]->lifespan.Finished()) {
            delete bullets_[i];
            bullets_.erase(bullets_.begin() + i);
            delete bullettrails_[i];
            bullettrails_.erase(bullettrails_.begin() + i);
            
        }
        else {
            bullets_[i]->Update(delta_time);
            GameObject* hit = nullptr;
            int hitIndex = -1;
            float minTime = INFINITY;
            for (int i2 = 1; i2 < game_objects_.size()-2; i2++) {
                if (game_objects_[i2]->type != ENEMY) {
                    return;
                }
                glm::vec3 directionVector = (-bullets_[i]->GetRight() * bullets_[i]->speed * (float)delta_time);
                float A = glm::dot(directionVector, directionVector);
                float B = glm::dot(2.0f * directionVector, (bullets_[i]->GetPosition() - game_objects_[i2]->GetPosition()));
                float C = glm::dot((bullets_[i]->GetPosition() - game_objects_[i2]->GetPosition()), (bullets_[i]->GetPosition() - game_objects_[i2]->GetPosition())) - pow(0.4f, 2.0f);
                float discriminant = pow(B, 2) - 4 * A * C;
                if (discriminant > 0) {
                    float firstSol = (-B + sqrt(discriminant)) / 2 * A;
                    float secondSol = (-B - sqrt(discriminant)) / 2 * A;
                    if (firstSol > 0 && secondSol < 0) {
                        if (secondSol < minTime) {
                            hitIndex = i2;
                            hit = game_objects_[i2];
                            minTime = secondSol;
                        }
                    }
                    else if (firstSol < 0 && secondSol < 0) {
                        B = glm::dot(2.0f * directionVector, (bullets_[i]->prevPosition - game_objects_[i2]->GetPosition()));
                        C = glm::dot((bullets_[i]->prevPosition - game_objects_[i2]->GetPosition()), (bullets_[i]->prevPosition - game_objects_[i2]->GetPosition())) - pow(0.4f, 2.0f);
                        float discriminant = pow(B, 2) - 4 * A * C;
                        firstSol = (-B + sqrt(discriminant)) / 2 * A;
                        secondSol = (-B - sqrt(discriminant)) / 2 * A;
                        if (firstSol > 0) {
                            if (secondSol < minTime) {
                                hitIndex = i2;
                                hit = game_objects_[i2];
                                minTime = secondSol;
                            }
                        }
                    }
                    std::cout << firstSol << " roots " << secondSol << " for " << i2 << std::endl;

                }
            }

            if (hit != nullptr) {
                std::cout << "hit" << std::endl;

                explosion_origins_.push_back(new GameObject(game_objects_[hitIndex]->GetPosition(), sprite_, &sprite_shader_, tex_[4]));
                explosion_origins_[explosion_origins_.size() - 1]->lifespan = Timer(4);
                explosions_.push_back(new ParticleSystem(glm::vec3(0.0f, 0.0f, 0.0f), explosiontrail_, &particle_shader_, tex_[4], explosion_origins_[explosion_origins_.size()-1]));
                explosions_[explosions_.size() - 1]->SetScale(0.2f);
                explosions_[explosions_.size() - 1]->spawnTime = current_time_;
                

                delete bullets_[i];
                bullets_.erase(bullets_.begin() + i);
                delete game_objects_[hitIndex];
                game_objects_.erase(game_objects_.begin() + hitIndex);
                delete bullettrails_[i];
                bullettrails_.erase(bullettrails_.begin() + i);

               
            }
            
        }
    }
}


void Game::Render(void) {
    if (player == nullptr) {
        return;
    }
    // Clear background
    glClearColor(viewport_background_color_g.r,
        viewport_background_color_g.g,
        viewport_background_color_g.b, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use aspect ratio to properly scale the window
    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    glm::mat4 window_scale_matrix;
    if (width > height) {
        float aspect_ratio = ((float)width) / ((float)height);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / aspect_ratio, 1.0f, 1.0f));
    }
    else {
        float aspect_ratio = ((float)height) / ((float)width);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f / aspect_ratio, 1.0f));
    }

    // Set view to zoom out, centered by default at 0,0
    float camera_zoom = 0.25f;
    glm::mat4 camera_zoom_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(camera_zoom, camera_zoom, camera_zoom));
    glm::mat4 view_matrix = window_scale_matrix * camera_zoom_matrix;
    if (game_objects_.size() > 0) {
        view_matrix = glm::translate(view_matrix, -1.0f * (playerContainer->GetPosition()));
    }

    blade->Render(view_matrix, current_time_);

    for (int i = 0; i < bullets_.size(); i++) {
        bullets_[i]->Render(view_matrix, current_time_);
    }
    // Render all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
        game_objects_[i]->Render(view_matrix, current_time_);
    }

    for (int i = 0; i < explosions_.size(); i++) {
        explosions_[i]->GetShader()->SetUniform1f("spawnTime", explosions_[i]->spawnTime);
        explosions_[i]->GetShader()->SetUniform1i("isExplosion", 1);
        explosions_[i]->Render(view_matrix, current_time_);
    }

    if (explosions_.size() > 0) {
        explosions_[0]->GetShader()->SetUniform1i("isExplosion", 0);
    }

    for (int i = 0; i < bullettrails_.size(); i++){
        bullettrails_[i]->GetShader()->SetUniform1i("isBlue", 1);
        bullettrails_[i]->Render(view_matrix, current_time_);
    }

    if (bullettrails_.size() > 0) {
        bullettrails_[0]->GetShader()->SetUniform1i("isBlue", 0);
    }
    

}
      
} // namespace game
