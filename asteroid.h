#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

class Asteroid {
public:
    ofVec3f position;
    float scale;
    bool alive;
    static ofxAssimpModelLoader model;  // Shared 3D model for all asteroids

    // Constructor - set position and random scale
    Asteroid(const ofVec3f &pos) {
        position = pos;
        scale = ofRandom(0.4, 2.0);   // Random scale between 40% and 200%
        alive = true;
    }

    // Load the 3D model once for all asteroids
    static void loadModel() {
        if (!model.loadModel("models/asteroid/asteroid.fbx", true)) {
            ofLogWarning("Asteroid") << "Missing or invalid models/asteroid/asteroid.fbx";
        }

        model.setScaleNormalization(false);
        model.setScale(0.01, 0.01, 0.01);

        model.setRotation(0, 90, 1, 0, 0);
    }

    // Draw the asteroid
    void draw() const {
        if(!alive) return;  // Don't draw if asteroid is destroyed

        ofPushMatrix();
        ofTranslate(position);  // Move to asteroid position

        ofScale(scale, scale, scale);

        model.drawFaces();  // Draw the 3D model

        ofPopMatrix();
    }
};