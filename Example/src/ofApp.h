#pragma once

#include "ofMain.h"
#include "ofxPeoplePower.h"

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
	
    ofxPeoplePower ofxPeoplePower;
    
    ofxXmlSettings XML;
    ofTrueTypeFont TTF;
    
    string xmlStructure;
    string message;
    string temp;
    
    string ppUsername;
    string ppPassword;
    
    void setupXML() {
        XML.popTag();
        XML.addTag("profile");
        XML.pushTag("profile");
        XML.addTag("email");
        XML.addTag("key");
        XML.addAttribute("key", "expires", "", 0);
        XML.addTag("user_id");
        XML.addTag("location_id");
        XML.addTag("devices");
        XML.popTag();
    }
    
    void XMLTranslate(string tag) {
        for (int i = 0; i < ofxPeoplePower.XML.getNumTags(tag); i++) {
            XML.addTag(tag);
            for (int j = 0; j < ofxPeoplePower.XML.getNumAttributes(tag); j++) {
                vector<string> attributeNames;
                
                ofxPeoplePower.XML.getAttributeNames(tag, attributeNames);
                XML.setAttribute(tag, attributeNames[j], ofxPeoplePower.XML.getAttribute(tag,attributeNames[j],"null", i), i);
            }
        }
    }
};
