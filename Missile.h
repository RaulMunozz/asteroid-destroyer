#pragma once
#include "ofMain.h"

class Missile {
public:
    // Position, velocity, and timing variables
    ofVec3f pos;
    ofVec3f vel;
    float birthTime;
    float lifeTime;

    // Default constructor
    Missile() {
        pos.set(0,0,0);
        vel.set(0,0,0);
        birthTime = ofGetElapsedTimef();
        lifeTime = 3.0f;
    }

    // Constructor with starting position and velocity
    Missile(const ofVec3f &start, const ofVec3f &velocity, float life = 3.0f) {
        pos = start;
        vel = velocity;
        birthTime = ofGetElapsedTimef();
        lifeTime = life;
    }

    // Update missile position based on velocity and time
    void update(float dt) {
        pos += vel * dt;
    }

    // Check if missile should be removed
    bool isDead() const {
        return (ofGetElapsedTimef() - birthTime) > lifeTime;
    }

    // Draw the missile as an orange sphere
    void draw() const {
        ofPushStyle();
        ofSetColor(255,160,80);  // Orange color
        ofDrawSphere(pos, 6);    // Draw sphere at position
        ofPopStyle();
    }
};