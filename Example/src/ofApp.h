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
    
    void getDeviceData() {
        // Clear previous devices
        XML.clearTagContents("profile:devices");
        
        // Set level to devices tag
        XML.pushTag("profile",0);
        XML.pushTag("devices",0);
        
        //Temporarily hardcode devices for my key --- TODO: Call from cloud - Currently devices must be gathered from website therefor are not up-to-date.
        // PeoplePowerApi.devices("location_id","user_id","false");
        ofxXmlSettings devicesXML = ofxXmlSettings("devices.xml");
        
        devicesXML.pushTag("response",0);
        devicesXML.pushTag("devices",0);
        //    cout << "getNumTags: " << devicesXML.getNumTags("device") << endl;
        
        // iterate through tags, copy to XML and set attributes
        for (int i = 0; i < devicesXML.getNumTags("device"); i++ ) {
            XML.addValue("device", devicesXML.getAttribute("device","desc","null", i));
            // TODO: enumerate through number of attributes and setAttributes to device
            XML.setAttribute("device", "id", devicesXML.getAttribute("device","id","null", i), i);
            XML.setAttribute("device", "connected", devicesXML.getAttribute("device","connected","null", i), i);
            XML.setAttribute("device", "lastDataReceivedDate", devicesXML.getAttribute("device","lastDataReceivedDate","null", i), i);
            XML.setAttribute("device", "lastMeasureDate", devicesXML.getAttribute("device","lastMeasureDate","null", i), i);
            XML.setAttribute("device", "type", devicesXML.getAttribute("device","type","null", i), i);
            XML.setAttribute("device", "typeCategory", devicesXML.getAttribute("device","typeCategory","null", i), i);
        }
        devicesXML.popTag();
        devicesXML.popTag();
        XML.popTag();
        XML.popTag();
    }
};
