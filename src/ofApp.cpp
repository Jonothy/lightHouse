#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    // need a xml settings to load all variables, possibly save
    loadParameters("settings.xml");
    
    receiver.setup(9999);
    // for debugging use - in case we want to send fake sensor info to ourselves
    sender.setup("localhost", 9999);
    
    threshold = 600;
    
    saveParameters();
    
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
    
    for(int i = 0; i < numSectors; i++){
        sectors[i]->setColor(globalColor());
        sectors[i]->update();
    }
    
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        vector<string> addressParsed = ofSplitString(m.getAddress(), "/");
        
        int currentSector = ofToInt(addressParsed.back());
        int currentValue = ofToInt(m.getArgAsString(0));
//        int currentValue = m.getArgAsInt32(0);

        if (currentValue > sectors[currentSector]->threshold) {
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
    ofSetColor(0);
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
    ofxOscMessage m, m1;
    
    switch (key) {
            
        case OF_KEY_UP:
            threshold += 10;
            break;
            
        case OF_KEY_DOWN:
            threshold -= 10;
            break;
            
        case 's':
            
            m.setAddress("/soundfile");
            m.addStringArg("synth.wav");
            sender.sendMessage(m);
            
            break;
            
        case '1':
            m1.setAddress("/sendTo");
            m1.addStringArg("192.168.0.191");
            sender.sendMessage(m1);
            
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
    m.addStringArg("1000");
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
        sectors = new lightSector*[numSectors];
    }
    globalTransition = settings.getValue<int>("GLOBALTRANSITION");
    globalBrightness = settings.getValue<int>("GLOBALBRIGHTNESS");
    settings.setTo("SECTORS");
    for(int i = 0; i < numSectors; i++){
        settings.setTo("SECTOR" + ofToString(i));
        ofLog() << i << endl;
        ofLog() << settings.getValue<string>("IP") << endl;
        ofLog() << settings.getValue<int>("PORT") << endl;
        ofLog() << settings.getValue<int>("THRESHOLD") << endl;
        
        string tempIP = settings.getValue<string>("IP");
        int tempPort = settings.getValue<int>("PORT");
        int tempThreshold = settings.getValue<int>("THRESHOLD");
        int tempTransitionTime = settings.getValue<int>("TRANSITIONTIME");
        int tempHoldTime = settings.getValue<int>("HOLDTIME");
        string tempSoundFile = settings.getValue<string>("SOUND");
        float tempMaxVolume = settings.getValue<float>("MAXVOL");
        
        settings.setTo("//SECTORS/SECTOR"+ofToString(i)+"/COLOR");
        ofLog() << settings.getValue<int>("R") << ","
        << settings.getValue<int>("G") << ","
        << settings.getValue<int>("B") << endl;
        
        int tempR = settings.getValue<int>("R");
        int tempG = settings.getValue<int>("G");
        int tempB = settings.getValue<int>("B");
        
        settings.setToParent(2);
        
        
        sectors[i] = new lightSector(tempIP, tempPort, i, tempThreshold, tempSoundFile, tempMaxVolume, tempTransitionTime, tempHoldTime, ofColor(tempR,tempG,tempB));
    }
}

// this was used to create xml template, can be re-adapted for actual saving if need be
void ofApp::saveParameters() {
    settings.clear();
    settings.addChild("PARAMS");
    settings.addValue("NUMSECTORS", ofToString(numSectors));
    settings.addValue("GLOBALTRANSITION", globalTransition);
    settings.addValue("GLOBALBRIGHTNESS", globalBrightness);
    settings.addChild("SECTORS");
    settings.setTo("//SECTORS");
    for(int i = 0; i < numSectors; i++){
        settings.addChild("SECTOR" + ofToString(i));
        settings.setTo("//SECTOR"+ ofToString(i));
        settings.addValue("IP", sectors[i]->sendIP);
        settings.addValue("PORT", ofToString(sectors[i]->sendPort));
        settings.addValue("THRESHOLD", sectors[i]->threshold);
        settings.addValue("TRANSITIONTIME", sectors[i]->transitionTime);
        settings.addValue("HOLDTIME", sectors[i]->holdTime);
        settings.addValue("SOUND", sectors[i]->soundFile);
        settings.addValue("MAXVOL", sectors[i]->maxVolume);
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
    
    int switcher = ofMap(currentTime % globalTransition, 0, globalTransition, 0, 765);
    
//    return wheel(switcher);
    ofColor adjustedColor = wheel(switcher);
    adjustedColor.setBrightness(globalBrightness);
    return adjustedColor;
}

// does a color wheel step by step
ofColor ofApp::wheel(int WheelPos) {
    WheelPos = 765 - WheelPos;
    if(WheelPos < 255) {
        return ofColor(255 - WheelPos, 0, WheelPos);
    }
    if(WheelPos < 510) {
        WheelPos -= 255;
        return ofColor(0, WheelPos, 255 - WheelPos);
    }
    WheelPos -= 510;
    return ofColor(WheelPos, 255 - WheelPos, 0);
}
