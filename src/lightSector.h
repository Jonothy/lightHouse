//
//  lightSector.h
//  lightHouse
//
//  Created by Johnny Lu on 11/4/15.
//
//

#ifndef _lightSector
#define _lightSector

#include "ofMain.h"
#include "ofxOsc.h"

class lightSector {
    
    public:
    
        void update();
        void draw();
        void setIP(string _ipAddress);
        void setPort(int _portNumber);
        void setSoundFile(string _filePath);
        void setSector(int _sectorNumber);
        void setColor(ofColor _color);
        void setActive();
        void setThreshold(int _threshold);
        float getVolume();
    
        void sendColor();
    
        lightSector(string _ipAddress, int _portNumber, int _sectorNumber, ofColor _selfColor);
    
        string sendIP;
        int sendPort;
    
        int sectorNumber;
        int threshold;
        float volume;
    
        bool active;
        long long timeActivated;
        long long lastActivated;
        string soundFile;
    
        ofColor selfColor;
        ofColor currentColor;
        ofColor prevColor;
    
        ofxOscSender sender;
    
        ofSoundPlayer sounder;
};

#endif