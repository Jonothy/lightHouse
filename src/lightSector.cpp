//
//  lightSector.cpp
//  lightHouse
//
//  Created by Johnny Lu on 11/4/15.
//
//

#include "lightSector.h"

lightSector::lightSector(string _ipAddress, int _portNumber, int _sectorNumber, ofColor _selfColor) {
    
    sender.setup(_ipAddress, _portNumber);
    sendIP = _ipAddress;
    sendPort = _portNumber;
    sectorNumber = _sectorNumber;
    
    volume = 0.0;
    active = false;
    
    lastActivated = 0;
    timeActivated = 0;
    selfColor = _selfColor;
    currentColor = ofColor(0,0,0);
    prevColor = ofColor(0,0,0);
}

void lightSector::update() {
    
    long long currentTime = ofGetSystemTime();
    if(currentTime - lastActivated > 5000){
        active = false;
        float elapsed = currentTime - lastActivated;
        
        if(elapsed > 7000){
            volume = 0;
        }
        else{
            volume = ofMap(elapsed, 5000, 7000, 1.0, 0);
            currentColor.lerp(selfColor, 1 - (elapsed-5000)/2000.0);
        }
    }
    else{
        float elapsed = currentTime - timeActivated;
        if(elapsed > 2000){
            volume = 1;
            currentColor = selfColor;
            ofLog() << int(currentColor.r) << "," << int(currentColor.g) << "," << int(currentColor.b) << endl;
        }
        else{
            volume = ofMap(elapsed, 0, 2000, 0.000, 1.0);
            currentColor.lerp(selfColor, elapsed/2000.0);
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

void lightSector::setThreshold(int _threshold){
    threshold = _threshold;
}

float lightSector::getVolume(){
    return volume;
}

void lightSector::sendColor(){
    ofxOscMessage m;
    m.setAddress("/led");
    m.addIntArg(int(currentColor.r));
    m.addIntArg(int(currentColor.g));
    m.addIntArg(int(currentColor.b));
    sender.sendMessage(m);
}