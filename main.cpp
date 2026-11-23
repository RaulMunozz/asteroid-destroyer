#include "ofMain.h"
#include "ofApp.h"

int main(){
    // Set up OpenGL window: 1600x900 pixels, windowed mode
    ofSetupOpenGL(1600, 900, OF_WINDOW);

    // Create and run the main application
    ofRunApp(new ofApp());

    // Exit program
    return 0;
}