#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "lightSector.h"

#define NUM_MSG_STRINGS 20

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ofColor globalColor();
        ofColor wheel(int WheelPos);
    
        ofxOscReceiver receiver;
        ofxOscSender sender;
        string sendAddress;
    
        lightSector **sectors;
        int numSectors;
    
        ofTrueTypeFont font;
        ofSoundPlayer sounder;
        ofSoundPlayer sounder2;
    
        int threshold;
    
        int current_msg_string;
        string msg_strings[NUM_MSG_STRINGS];
        float timers[NUM_MSG_STRINGS];
};
