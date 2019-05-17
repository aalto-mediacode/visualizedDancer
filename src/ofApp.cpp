#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableDepthTest();
    ofBackground(0);
    
    receiver.setup(12345);
    currentMsgString = 0;
    
    stageX = 500; // ofGetWidth();
    stageY = 500; // ofGetHeight();
    stageZ = 500;
    
    rotationAngle = 45;
    //glPointSize(3); // make the points bigger
    
    boolShowDancer = true;
    boolShowFirefly = false;
    boolShowTriangles = false;
    boolShowLines = false;
    boolShowLinesAndPoints = false;
    boolShowThinLines = false;
    boolShowFirstLines = false;

    
    for (int i=0; i < NUM_MSG_STRINGS/3; i++) {
        myMeshes[i].setMode(OF_PRIMITIVE_LINE_LOOP);
    }
    
    //mesh.enableColors();
    
    glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
    glPointSize(10); // make the points bigger
    
    skip = 0;
    myIndex = 1;
    
    allPoints.setMode(OF_PRIMITIVE_POINTS);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    // animating the rotation angle
    rotationAngle = (rotationAngle + 0.1); // % 360;
    
    //Line drawing test
    ofPoint pt;
    for (int i=10; i< 120; i++) {
        pt.set(i, i*1.3, i*2);
        line.addVertex(pt);
    }
    
    
    
    while (receiver.hasWaitingMessages()) {
        ofxOscMessage msg;
        receiver.getNextMessage(msg);
     
        string msgString;
        msgString = msg.getAddress();
        msgString += ":";
        
        // storing the data in vectors
        string dataName = msg.getAddress();
        // just the first character: e.g. x
        string axis = dataName.substr (1,1);
        // from the second character till the end and parse it to int
        string number = dataName.substr(2,-1);
        

        if (msg.getArgType(0) == OFXOSC_TYPE_FLOAT){
            
            // Testing ----------------------
            if((axis=="x" || axis=="y") && number=="2"){
                //cout << "axis: " + axis + " - number: " << number << ": " << msg.getArgAsFloat(0) << endl;
            }
            if (axis=="x" && number=="1") myVector.x = msg.getArgAsFloat(0);
            if (axis=="y" && number=="1") myVector.y = msg.getArgAsFloat(0);
            // -----------------------------
            
            int index = ofToInt(number);
            if (index != 0) {
                if (axis == "x") {
                    vectordata[index-1].x = msg.getArgAsFloat(0);
                } else if (axis == "y") {
                    vectordata[index-1].y = msg.getArgAsFloat(0);
                } else if (axis == "z") {
                    vectordata[index-1].z = msg.getArgAsFloat(0);
                }
                //movement.addVertex(vectordata[index-1]); //.addVertex(x,y,0);
                //if (index == 1) {
                    ofPoint pt;
                    if ((vectordata[index-1].x != 0) &&
                        (vectordata[index-1].y != 0) &&
                        (vectordata[index-1].z !=0)) {
                            
                        pt.set(ofMap(vectordata[index-1].x, RANGE_FROM, RANGE_TO, -stageX, stageX),
                           ofMap(vectordata[index-1].y, RANGE_FROM, RANGE_TO, -stageY, stageY),
                           ofMap(vectordata[index-1].z, RANGE_FROM, RANGE_TO, -stageZ, stageZ));
                        
                        // TEST 1
                        //myLines[index-1].addVertex(pt);
                        
                        // TEST 2
                        myLines[index-1].curveTo(pt, 1000);
                        
                        // add all points to this mesh
                        allPoints.addVertex(pt);
                        allPoints.addColor(ofColor(0,0,255));
                        
                        //allPointsArray.add(pt);
                        // add a random position to all the points
                        ofPoint pt_random;
                        pt_random.set(ofRandom(-FIREFLY_DISTANCE,FIREFLY_DISTANCE) + ofMap(vectordata[index-1].x, RANGE_FROM, RANGE_TO, -stageX, stageX),
                               ofRandom(-FIREFLY_DISTANCE,FIREFLY_DISTANCE) + ofMap(vectordata[index-1].y, RANGE_FROM, RANGE_TO, -stageY, stageY),
                               ofRandom(-FIREFLY_DISTANCE,FIREFLY_DISTANCE) + ofMap(vectordata[index-1].z, RANGE_FROM, RANGE_TO, -stageZ, stageZ));
                        
                        allPointsArray.push_back(pt_random);
                        allPointsArrayAlpha.push_back(FIREFLY_ALPHA);
                        
                        
                        if (skip % SKIP_NUMBER == 0) {
                            //ofVec3f pos(x, y, z);
                            mesh.addVertex(pt);
                            //mesh.addColor(ofFloatColor(1.0, 0.0, 0.0)); // Red
                            //mesh.addIndex(myIndex);
                            
                            myMeshes[index-1].addVertex(pt);
                            
                            myIndex++;
                        }
                        skip++;
                        
                    }
                //}
                
            }
        }
        
        
        
        
        
        
        // FOR TESTING OUTPUT -------
        for(int i = 0; i < msg.getNumArgs(); i++){
            // get the argument type
            msgString += " ";
            msgString += msg.getArgTypeName(i);
            msgString += ":";
            
            if (msg.getArgType(i) == OFXOSC_TYPE_FLOAT){
                msgString += ofToString(msg.getArgAsFloat(i));
            }
            
            // add to the list of strings to display
            msgStrings[currentMsgString] = msgString;
            currentMsgString = (currentMsgString + 1) % NUM_MSG_STRINGS;
            // clear the next line
            msgStrings[currentMsgString] = "";
        }
        //------------------------------
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //for 3d:
    cam.begin();
    // scale the 3 axis
    ofScale(1,1,1);
    // nach unten verschieben
    ofTranslate(ofVec3f(0,-stageY/2,0));
    // etwas nach links drehen
    ofRotateYDeg(rotationAngle);
    
    
    //line.draw();
    
    // TEST 1 Draw the lines between the vectors
    if (boolShowFirstLines) {
        ofSetColor(255, 255, 255, 50);
        for (int i=0; i < NUM_MSG_STRINGS/3; i++) {
            //myLines[i].getSmoothed(2).draw();
            glLineWidth(1);
            ofSetColor(255, 255, 255, 50);
            myLines[i].draw();
        }
    }
//
    // TEST 2 draw a bounding box around one polyline
//    ofSetColor(255, 0, 0, 20);
//    ofDrawRectangle(myLines[1].getBoundingBox());

//    for (int i=0; i < NUM_MSG_STRINGS/3; i++) {
//        ofDrawRectangle(myLines[i].getBoundingBox());
//    }
    
    
    // TEST 3 drawing the vertices (triangle) without a line in between them
    if (boolShowTriangles) {
        //mesh.setMode(OF_PRIMITIVE_POINTS);
        //mesh.setMode(OF_PRIMITIVE_TRIANGLES);
        //mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        
        ofSetColor(255, 255, 255, 50);
       for (int i=0; i < NUM_MSG_STRINGS/3; i++) {

            mesh.draw();
           
    //       for (int j = 0; j < mesh.getVertices().size(); j++) {
    //           ofDrawLine(mesh.getVertices()[j].x,
    //       }
           
           
    //       for (int j = 0; j < myLines[i].getVertices().size(); j++) {
    //           ofDrawSphere( myLines[i].getVertices()[j], 10);
    //       }
       }
    }
    
    // TEST 3.1 drawing a line between all of them
    if (boolShowLines || boolShowThinLines) {
        mesh.setMode(OF_PRIMITIVE_LINES);
        //mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
        //mesh.setMode(OF_PRIMITIVE_LINE_LOOP);
        
        for (int i=0; i < NUM_MSG_STRINGS/3; i++) {
            if (boolShowThinLines) {
                glLineWidth(1);
                ofSetColor(255, 255, 255, 50);
            } else if (boolShowLines) {
                glLineWidth(30);
                ofSetColor(255, 255, 255, 20);
            }
            mesh.draw();
        }
    }
    
    
    // TEST 4 draw a mesh for each data-point
    if (boolShowLinesAndPoints) {
        for (int i=0; i < NUM_MSG_STRINGS/3; i++) {
            myMeshes[i].setMode(OF_PRIMITIVE_LINE_LOOP); // Lines
            glLineWidth(1);
            ofSetColor(255, 255, 255, 50);
            myMeshes[i].draw();
            glPointSize(10); //  glPointSize(10); or 6
            myMeshes[i].setMode(OF_PRIMITIVE_POINTS); // Points
            ofSetColor(255, 255, 255, 50);
            myMeshes[i].draw();
        }
    }
    
    
    // TEST 5 draw all data points
    if (boolShowFirefly) {
        glPointSize(2);
       // allPoints.draw();
        
    //    for (ofVec3f vec : allPoints.getVertices()) {
    //        vec.x += ofRandom(-10,10);
    //        vec.y += ofRandom(-10,10);
    //        vec.z += ofRandom(-10,10);
    //        ofSetColor(255, 255, 255, 100);
    //        //ofDrawSphere(vec.x, vec.y, vec.z, 1);
    //    }
        allPointsArrayMesh.clear();
        for (int i=0; i<allPointsArray.size(); i++) {
            allPointsArray[i].x += ofRandom(-0.3,0.3);
            allPointsArray[i].y += ofRandom(-0.3,0.3);
            allPointsArray[i].z += ofRandom(-0.3,0.3);
            allPointsArrayAlpha[i] -= 0.01;
            if (allPointsArrayAlpha[i] < 1) {
            //    allPointsArray.erase(i);
            //    allPointsArrayAlpha.erase(i);
            }
            ofSetColor(255, 255, 255, allPointsArrayAlpha[i]);
          //  ofDrawSphere(allPointsArray[i].x, allPointsArray[i].y, allPointsArray[i].z, 1);
            
            if (allPointsArrayAlpha[i] > 0) {
                allPointsArrayMesh.addVertex(ofVec3f(allPointsArray[i].x, allPointsArray[i].y, allPointsArray[i].z));
                allPointsArrayMesh.addColor(ofColor(255,255,255,allPointsArrayAlpha[i]));
            }
        }
        
        allPointsArrayMesh.setMode(OF_PRIMITIVE_POINTS); // Points
        glPointSize(1);
        allPointsArrayMesh.draw();
    }
    
    
    
    
    // draw Polyline
    //movement.draw();
    
    //ofSetLineWidth(2.0);
    //ofDrawLine(vectordata[0].x, vectordata[0].y, vectordata[2].x, vectordata[2].y);
    
//    quad.addVertex(vectordata[1]);
//    quad.addVertex(vectordata[2]);
//    quad.addVertex(vectordata[3]);
//    quad.addVertex(vectordata[4]);
//
//    quad.draw();
    
    
   
    
//    startingPoint.x = vectordata[0].x;
//    startingPoint.y = vectordata[0].y;
//    startingPoint.z = vectordata[0].z;
//
//    // move the coordinate system to position of point and make that zero
//    ofTranslate(startingPoint);
    
    
    
    //ofTranslate(-img.getWidth() / 2, -img.getHeight() / 2);
    //mesh.draw();
    
    
    for (int i = 0; i < NUM_MSG_STRINGS/3; i++) {
        
        // 2D drawing:
        //ofDrawCircle(ofMap(vectordata[i].x, RANGE_FROM, RANGE_TO, 0, stageWidth), ofMap(vectordata[i].y, RANGE_FROM, RANGE_TO, 0, stageWidth), 20);
        
        // 3D drawing
        
        // Draw the axis --------
//        ofSetColor(255, 255, 255);
//        ofDrawBox(15);
//        // x - red
//        ofSetColor(255, 0, 0);
//        ofDrawBox(50, 0, 0, 10);
//        // y - green
//        ofSetColor(0, 255, 0);
//        ofDrawBox(0, 50, 0, 10);
//        // z - blue
//        ofSetColor(0, 0, 255);
//        ofDrawBox(0, 0, 50, 10);
        //---------------------
        
        // draw Spheres where the Data points are
        ofSetColor(255, 255, 255);
        if (boolShowDancer) {
            ofDrawSphere(ofMap(vectordata[i].x, RANGE_FROM, RANGE_TO, -stageX, stageX),
                         ofMap(vectordata[i].y, RANGE_FROM, RANGE_TO, -stageY, stageY),
                         ofMap(vectordata[i].z, RANGE_FROM, RANGE_TO, -stageZ, stageZ),
                         10);
        }
        
        cout << "i: " << i << ", x: " << vectordata[i].x << ", y: " << vectordata[i].y << ", z: " << vectordata[i].z << endl;
    }
    
    
    
   // ofMap(i,0,NUM_BANDS,0,255);
    
    // DRAWING TEST VECTOR
    // ofDrawCircle(ofMap(myVector.x, RANGE_FROM, RANGE_TO, 0, stageWidth), ofMap(myVector.y, RANGE_FROM, RANGE_TO, 0, stageHeight), 30);
    
    
//    // PRINTING OUT VALUES OF x1, y1
//    cout << "ofVec2f: x: " << myVector.x << "/" << ofMap(myVector.x, 0, 1, 0, stageX) << "/" << stageX << ", y: " << myVector.y << "/" << ofMap(myVector.y, 0, 1, 0, stageY) << "/" << stageY << endl;
    
    
    
    // draw recent unrecognized messages
    for (int i = 0; i < NUM_MSG_STRINGS; i++) {
        //ofDrawBitmapStringHighlight(msgStrings[i], 10, 40 + 15 * i);
    }
    
    cam.end();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == '1') {
        boolShowDancer = !boolShowDancer;
    } else if (key == '4') {
        boolShowLines = !boolShowLines;
    } else if (key == '5') {
        boolShowLinesAndPoints = !boolShowLinesAndPoints;
    } else if (key == '6') {
        boolShowTriangles = !boolShowTriangles;
    } else if (key == '7') {
        boolShowFirefly = !boolShowFirefly;
    } else if (key == '3') {
        boolShowThinLines = !boolShowThinLines;
    } else if (key == '2') {
        boolShowFirstLines = !boolShowFirstLines;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
