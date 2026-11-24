#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Missile.h"
#include "asteroid.h"
#include <vector>

class ofApp : public ofBaseApp {
public:
    void setup() override;      // Initialize the game
    void update() override;     // Update game logic each frame
    void draw() override;       // Render the game

    void keyPressed(int key) override;  // Handle keyboard input

private:
    // 3D models
    ofxAssimpModelLoader spaceship;  // Player spaceship model

    // Ship properties
    ofVec3f shipPos;    // Current ship position
    float shipSpeed;    // Ship movement speed

    // Cameras for different views
    ofCamera camMain;   // Main camera view
    ofCamera camFront;  // Front view camera
    ofCamera camBack;   // Rear view camera
    ofCamera camTop;    // Top view camera

    // Lighting
    ofLight sun;        // Main directional light
    ofLight fillLight;  // Additional fill light

    // Game objects
    std::vector<Asteroid> asteroids;  // List of asteroids
    std::vector<Missile> missiles;    // List of active missiles

    // Sound effects
    ofSoundPlayer music;        // Background music
    ofSoundPlayer sndShoot;     // Shooting sound
    ofSoundPlayer sndExplosion; // Explosion sound

    // Skybox for background
    ofSpherePrimitive sky;      // Sphere for sky background
    ofImage skyTexture;         // Texture for sky

    // Background starfield
    ofImage starfield;          // Starfield image for space background
};