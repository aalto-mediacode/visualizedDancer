#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

// listening port
#define PORT 12345
// max number of strings to display
#define NUM_MSG_STRINGS 36

//Range of mapping the OSC data
#define RANGE_FROM -2.5
#define RANGE_TO 2.5

// Settings
#define SKIP_NUMBER 30 // 20 or 80
#define FIREFLY_ALPHA 40
#define FIREFLY_DISTANCE 40


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    float timeSent, timeReceived;
    string name;
    ofxOscReceiver receiver;
    
    int currentMsgString;
    string msgStrings[NUM_MSG_STRINGS];
    
    ofVec3f vectordata[NUM_MSG_STRINGS];
    
    ofVec2f myVector;
    
    int stageX, stageY, stageZ;
    
    ofEasyCam cam;
    
    ofPoint startingPoint;
    
    float rotationAngle;
    
    ofPolyline movement;
    
    ofMesh quad;
    
    ofPolyline line;
    ofPolyline myLines[NUM_MSG_STRINGS];
    
    ofMesh mesh;
    ofMesh myMeshes[NUM_MSG_STRINGS];
    int skip;
    
    int myIndex;
    
    ofMesh allPoints;
    vector <ofVec3f> allPointsArray;
    vector <float> allPointsArrayAlpha;
    
    ofMesh allPointsArrayMesh;
    
    // Setting Booleans
    bool boolShowDancer;
    bool boolShowFirefly;
    bool boolShowTriangles;
    bool boolShowLines;
    bool boolShowLinesAndPoints;
    bool boolShowFirstLines;
    bool boolShowThinLines;
};
