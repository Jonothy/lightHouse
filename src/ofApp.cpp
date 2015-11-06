#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    // need a xml settings to load all variables, possibly save
    loadParameters("settings.xml");
    
    
    numSectors = 2;
    sectors = new lightSector*[numSectors];
    
    sectors[0] = new lightSector("192.168.1.101", 9998, 0, ofColor(255,0,0));
    sectors[1] = new lightSector("192.168.0.211", 12345, 1, ofColor(0,10,240));
    
    sounder.loadSound("sounds/synth.wav");
    sounder2.loadSound("sounds/Violet.mp3");
    
    sounder.setLoop(true);
    sounder2.setLoop(true);
    
    sounder.setVolume(0.5f);
    sounder2.setVolume(0.5f);
    
    sounder.setPan(-1.0f);
    sounder2.setPan(1.0f);
    
    sounder.play();
    sounder2.play();
    
    receiver.setup(9999);
    sender.setup("localhost", 9999);
    
    threshold = 600;
    
//    saveParameters();
    
//    loadParameters("settings.xml");
    ofSetFrameRate(30);
}

//--------------------------------------------------------------
void ofApp::update(){
    // hide old messages
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
        if(timers[i] < ofGetElapsedTimef()){
            msg_strings[i] = "";
        }
    }
    
    sectors[0]->setColor(globalColor());
    sectors[1]->setColor(globalColor());
    
    for(int i = 0; i < numSectors; i++){
        sectors[i]->update();
    }
    
    
    
    sounder.setVolume(sectors[0]->getVolume());
    sounder2.setVolume(sectors[1]->getVolume());
    
    ofSoundUpdate();
    
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        vector<string> addressParsed = ofSplitString(m.getAddress(), "/");
        
        int currentSector = ofToInt(addressParsed.back());
        int currentValue = m.getArgAsInt32(0);

        if (currentValue > threshold) {
            sectors[currentSector]->setActive();
        }
        
        
        
        // messages display on the bottom of the screen
        string msg_string;
        msg_string = m.getAddress();
        msg_string += ": ";
        for(int i = 0; i < m.getNumArgs(); i++){
            // get the argument type
            msg_string += m.getArgTypeName(i);
            msg_string += ":";
            // display the argument - make sure we get the right type
            if(m.getArgType(i) == OFXOSC_TYPE_INT32){
                msg_string += ofToString(m.getArgAsInt32(i));
            }
            else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
                msg_string += ofToString(m.getArgAsFloat(i));
            }
            else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
                msg_string += m.getArgAsString(i);
            }
            else{
                msg_string += "unknown";
            }
        }
        // add to the list of strings to display
        msg_strings[current_msg_string] = msg_string;
        timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
        current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
        // clear the next line
        msg_strings[current_msg_string] = "";
        
    }
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    for(int i = 0; i < numSectors; i++){
        sectors[i]->draw();
    }
//    ofBackground(allColor);
    string buf;
    buf = "listening for osc messages on port " + ofToString(9999);
    ofDrawBitmapString(buf, 10, 20);
    string currentThresh;
    currentThresh = "threshold is at: " + ofToString(threshold) + " "
        + ofToString(sectors[0]->volume);
    ofDrawBitmapString(currentThresh, 10, 30);
    
    
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
        ofDrawBitmapString(msg_strings[i], 10, 40 + 15 * i);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
            
        case OF_KEY_UP:
            threshold += 10;
            break;
            
        case OF_KEY_DOWN:
            threshold -= 10;
            break;
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
    ofxOscMessage m;
    if(y > ofGetWindowWidth() / 2){
        m.setAddress("/0");
    }
    else{
        m.setAddress("/1");
    }
    m.addIntArg(1000);
//    m.addIntArg(255);
//    m.addIntArg(128);
//    m.addIntArg(0);
    sender.sendMessage(m);
    ofLog() << "sent message" << endl;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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

void ofApp::loadParameters(string filePath) {
    settings.load(filePath);
    settings.setTo("PARAMS");
    if(settings.exists("NUMSECTORS")){
        ofLog() << "NUMSECTORS " << settings.getValue<int>("NUMSECTORS") << endl;
        numSectors = settings.getValue<int>("NUMSECTORS");
    }
    settings.setTo("SECTORS");
    for(int i = 0; i < numSectors; i++){
        settings.setTo("SECTOR" + ofToString(i));
        ofLog() << i << endl;
        ofLog() << settings.getValue<string>("IP") << endl;
        ofLog() << settings.getValue<int>("PORT") << endl;
        settings.setTo("//SECTORS/SECTOR"+ofToString(i)+"/COLOR");
        ofLog() << settings.getValue<int>("R") << ","
            << settings.getValue<int>("G") << ","
            << settings.getValue<int>("B") << endl;
        settings.setToParent(2);
        
    }
}

void ofApp::saveParameters() {
    settings.clear();
    settings.addChild("PARAMS");
    settings.addValue("NUMSECTORS", ofToString(numSectors));
    settings.addChild("SECTORS");
    settings.setTo("//SECTORS");
    for(int i = 0; i < numSectors; i++){
        settings.addChild("SECTOR" + ofToString(i));
        settings.setTo("//SECTOR"+ ofToString(i));
        settings.addValue("IP", sectors[i]->sendIP);
        settings.addValue("PORT", ofToString(sectors[i]->sendPort));
        settings.addChild("COLOR");
        settings.setTo("//SECTORS/SECTOR"+ofToString(i)+"/COLOR");
        settings.addValue("R", ofToString(int(sectors[i]->selfColor.r)));
        settings.addValue("G", ofToString(int(sectors[i]->selfColor.g)));
        settings.addValue("B", ofToString(int(sectors[i]->selfColor.b)));
        settings.setToParent(2);
    }
    settings.save("settings.xml");
}

ofColor ofApp::globalColor(){
    long long currentTime = ofGetSystemTime();
    
    int switcher = ofMap(currentTime % 20000, 0, 20000, 0, 255);
    
    return wheel(switcher);
}

ofColor ofApp::wheel(int WheelPos) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
        return ofColor(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if(WheelPos < 170) {
        WheelPos -= 85;
        return ofColor(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return ofColor(WheelPos * 3, 255 - WheelPos * 3, 0);
}
