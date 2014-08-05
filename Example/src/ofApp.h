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
    int textSpacing = 18;
    
    string ppUsername;
    string username;
    string ppPassword;
    string password;
    
    int setUsername;
    bool setPassword;
    
    bool signedIn;
    
    // Device Information
    int numberOfDevices;
    
    int t = 0;
    
    string targetString;
    
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
    
    void XMLSetTagAttributeToString(string tag, string attribute, int k) {

        
//        for (int i = 0; i < XML.getNumTags(tag); i++) {
            for (int j = 0; j < XML.getNumAttributes(tag); j++) {
                vector<string> attributeNames;
                
                XML.getAttributeNames(tag, attributeNames);
                if (attributeNames[j] == attribute) {
                    targetString = XML.getAttribute(tag,attributeNames[j],"null", k);
                }
            }
        }
//    }

};
