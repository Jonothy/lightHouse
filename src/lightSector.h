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
    
        lightSector(string _ipAddress, int _portNumber, int _sectorNumber, int _threshold, string _soundFile, float _maxVolume, int _transitionTime, int _holdTime, ofColor _selfColor);
    
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
    
        void sendVolume();
        void sendSoundPosition();
    
    
    
        string sendIP;
        int sendPort;
    
        int sectorNumber;
        int threshold;
    
        string soundFile;
        float volume;
        float maxVolume;
        float soundPosition;
    
        bool active;
        long long timeActivated;
        long long lastActivated;
    
        int transitionTime;
        int holdTime;
    
        ofColor selfColor;
        ofColor currentColor;
        ofColor prevColor;
    
        ofxOscSender sender;
    
        ofSoundPlayer sounder;
};

#endif