//
//  lightSector.cpp
//  lightHouse
//
//  Created by Johnny Lu on 11/4/15.
//
//

#include "lightSector.h"

lightSector::lightSector(string _ipAddress, int _portNumber, int _sectorNumber) {
    
    sender.setup(_ipAddress, _portNumber);
    sendIP = _ipAddress;
    sendPort = _portNumber;
    sectorNumber = _sectorNumber;
    
    active = false;
    
    currentColor = ofColor(0,0,0);
    prevColor = ofColor(0,0,0);
    
    //sounder.loadSound("filename");
    if(_sectorNumber == 0){
        sounder.setPan(-1.0f);
    }
    else{
        sounder.setPan(1.0f);
    }
    sounder.setVolume(0);
    sounder.play();
}

void lightSector::update() {
    long long currentTime = ofGetSystemTime();
    if(currentTime - lastActivated > 10000){
        active = false;
        float elapsed = currentTime - lastActivated;
        
        if(elapsed > 4000){
            sounder.setVolume(0);
        }
        else{
            sounder.setVolume(ofMap(elapsed, 0, 4000, 0.500, 0));
        }
    }
    else{
        float elapsed = currentTime - timeActivated;
        if(elapsed > 4000){
            sounder.setVolume(0.5);
        }
        else{
            sounder.setVolume(ofMap(elapsed, 0, 4000, 0.000, 0.500));
        }
    }
    
    sendColor();
}

void lightSector::draw() {
    ofPath path;
    path.arc(400,400, 200, 200, sectorNumber*40, (sectorNumber+1)*40);
    path.setCircleResolution(200);
    path.setColor(currentColor);
    path.setStrokeWidth(3);
    if(active){
        path.setStrokeColor(ofColor(255));
    }
    else{
        path.setStrokeColor(ofColor(100));
    }
    path.draw();
}

void lightSector::setIP(string _ipAddress) {
    sendIP = _ipAddress;
}

void lightSector::setPort(int _portNumber){
    sendPort = _portNumber;
}

void lightSector::setSoundFile(string _filePath){
    soundFile = _filePath;
}

void lightSector::setSector(int _sectorNumber){
    sectorNumber = _sectorNumber;
}

void lightSector::setColor(ofColor _color){
 
    prevColor = currentColor;
    currentColor = _color;
}

void lightSector::setActive(){
    long long currentTime = ofGetSystemTime();
    if(!active){
        active = true;
        timeActivated = currentTime;
        lastActivated = timeActivated;
    }
    else{
        lastActivated = currentTime;
    }
}

void lightSector::sendColor(){
    ofxOscMessage m;
    m.setAddress("/led");
    m.addIntArg(int(currentColor.r));
    m.addIntArg(int(currentColor.g));
    m.addIntArg(int(currentColor.b));
    sender.sendMessage(m);
}