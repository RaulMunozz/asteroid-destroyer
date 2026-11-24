#include "ofApp.h"

// Helper function to calculate distance between two points
static float dist(const ofVec3f &a, const ofVec3f &b) {
    return (a - b).length();
}

void ofApp::setup(){
    // Basic graphics settings
    ofSetFrameRate(60);
    ofBackground(8, 12, 20);
    ofEnableDepthTest();
    ofDisableArbTex();
    ofEnableLighting();

    // Load and setup spaceship model
    bool loaded = spaceship.loadModel("models/spaceship/spaceship.fbx", true);
    if (!loaded) {
        ofLogWarning("ofApp") << "Missing or invalid models/spaceship/spaceship.fbx";
    }
    spaceship.setScale(0.6, 0.6, 0.6);
    spaceship.setRotation(0, 180, 0, 1, 0);

    // Load asteroid model (shared by all asteroids)
    Asteroid::loadModel();

    // Initialize ship position and speed
    shipPos.set(0, 0, 0);
    shipSpeed = 18.0f;

    // Setup main directional light (sun)
    sun.setDirectional();
    sun.setDiffuseColor(ofFloatColor(1,1,1));
    sun.setSpecularColor(ofFloatColor(1,1,1));
    sun.setPosition(300,600,900);
    sun.lookAt(ofVec3f(0,0,-2000));
    sun.enable();

    // Setup fill light
    fillLight.setup();
    fillLight.setPointLight();
    fillLight.setDiffuseColor(ofFloatColor(0.3,0.3,0.35));
    fillLight.setPosition(0,200,300);
    fillLight.enable();

    // Setup cameras
    camMain.setNearClip(0.1f);
    camMain.setFarClip(10000);
    camFront.setNearClip(0.1f);
    camBack.setNearClip(0.1f);
    camTop.setNearClip(0.1f);

    // Create initial asteroids
    for(int i=0; i<40; i++){
        ofVec3f p(
            ofRandom(-1000,1000),
            ofRandom(shipPos.y - 200, shipPos.y + 200),
            ofRandom(-4000,-500)
        );
        asteroids.emplace_back(Asteroid(p));
    }

    // Load and play sounds
    music.load("sounds/music.mp3");
    music.setLoop(true);
    music.setVolume(0.6);
    music.play();

    sndShoot.load("sounds/shoot.wav");
    sndExplosion.load("sounds/explosion.wav");
}

void ofApp::update(){
    float dt = ofGetLastFrameTime();

    // Update missiles and remove dead ones
    for(int i = missiles.size()-1; i >= 0; i--){
        missiles[i].update(dt);
        if(missiles[i].isDead()){
            missiles.erase(missiles.begin() + i);
        }
    }

    // Move asteroids forward and recycle them when they pass the ship
    for(auto &a : asteroids){
        if(!a.alive) continue;

        a.position.z += 20 * dt;  // Move asteroid forward

        // Reset asteroid if it goes too far behind ship
        if(a.position.z > shipPos.z + 500){
            a.position = {
                ofRandom(-1000,1000),
                ofRandom(shipPos.y - 200, shipPos.y + 200),
                ofRandom(-4000, -300)
            };
            a.scale = ofRandom(0.4, 2.0);
        }
    }

    // Check for collisions between missiles and asteroids
    for(int i = missiles.size()-1; i >= 0; i--){
        for(int j = 0; j < asteroids.size(); j++){
            if(!asteroids[j].alive) continue;

            // Calculate hit radius based on asteroid scale
            float hitRadius = 100.0f * asteroids[j].scale;

            if(dist(missiles[i].pos, asteroids[j].position) < hitRadius){
                // Destroy asteroid and missile on collision
                asteroids[j].alive = false;
                if(sndExplosion.isLoaded()) sndExplosion.play();
                missiles.erase(missiles.begin() + i);
                break;
            }
        }
    }

    // Respawn dead asteroids every 7 seconds
    static float t = 0;
    t += dt;
    if(t > 7.0f){
        for(auto &a : asteroids){
            if(!a.alive){
                a.position = {
                    ofRandom(-1000,1000),
                    ofRandom(shipPos.y - 200, shipPos.y + 200),
                    ofRandom(-4000,-500)
                };
                a.scale = ofRandom(0.4,2.0);
                a.alive = true;
            }
        }
        t = 0;
    }

    // Update camera positions to follow ship
    camMain.setPosition(shipPos.x, shipPos.y + 80, shipPos.z + 250);
    camMain.lookAt(shipPos);

    camFront.setPosition(shipPos.x, shipPos.y + 30, shipPos.z - 120);
    camFront.lookAt(shipPos);

    camBack.setPosition(shipPos.x, shipPos.y + 30, shipPos.z + 400);
    camBack.lookAt(shipPos);

    camTop.setPosition(shipPos.x, shipPos.y + 600, shipPos.z);
    camTop.lookAt(shipPos);
}

void ofApp::draw(){
    int w = ofGetWidth();
    int h = ofGetHeight();

    // VIEWPORT 1 (main)
    ofViewport(0, 0, w/2, h/2);
    camMain.begin();
    spaceship.setPosition(shipPos.x, shipPos.y, shipPos.z);   // Position spaceship
    spaceship.drawFaces();
    for(const auto &a : asteroids) if(a.alive) a.draw();
    for(const auto &m : missiles) m.draw();
    camMain.end();

    // VIEWPORT 2 (front)
    ofViewport(w/2, 0, w/2, h/2);
    camFront.begin();
    spaceship.drawFaces();
    for(const auto &a : asteroids) if(a.alive) a.draw();
    for(const auto &m : missiles) m.draw();
    camFront.end();

    // VIEWPORT 3 (back)
    ofViewport(0, h/2, w/2, h/2);
    camBack.begin();
    spaceship.drawFaces();
    for(const auto &a : asteroids) if(a.alive) a.draw();
    for(const auto &m : missiles) m.draw();
    camBack.end();

    // VIEWPORT 4 (top)
    ofViewport(w/2, h/2, w/2, h/2);
    camTop.begin();
    for(const auto &a : asteroids) if(a.alive) a.draw();
    spaceship.drawFaces();
    for(const auto &m : missiles) m.draw();
    camTop.end();

    // Draw HUD (Heads Up Display)
    ofViewport(0,0,w,h);
    ofSetColor(255);
    string info = "Controls: Arrow keys to move | W/S up-down | SPACE shoot";
    ofDrawBitmapStringHighlight(info, 10, 20);
    ofDrawBitmapString("Missiles: " + ofToString(missiles.size()) +
                       "  Asteroids alive: " +
                       ofToString(std::count_if(
                           asteroids.begin(), asteroids.end(),
                           [](const Asteroid &a){ return a.alive; })),
                       10, 40);
}

void ofApp::keyPressed(int key){
    // Handle ship movement
    if(key == OF_KEY_UP)        shipPos.z -= shipSpeed;
    if(key == OF_KEY_DOWN)      shipPos.z += shipSpeed;
    if(key == OF_KEY_LEFT)      shipPos.x -= shipSpeed;
    if(key == OF_KEY_RIGHT)     shipPos.x += shipSpeed;
    if(key == 'w' || key == 'W') shipPos.y += shipSpeed;
    if(key == 's' || key == 'S') shipPos.y -= shipSpeed;

    // Shoot missile with spacebar
    if(key == ' '){
        missiles.emplace_back(
            Missile(
                shipPos + ofVec3f(0,0,-40),  // Start in front of ship
                ofVec3f(0,0,-500),           // Move forward
                3.0f                         // Lifetime in seconds
            )
        );
        if(sndShoot.isLoaded()) sndShoot.play();
    }
}