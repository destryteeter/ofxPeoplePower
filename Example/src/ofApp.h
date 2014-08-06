#pragma once

#include "ofMain.h"
#include "ofxPeoplePower.h"
#include "ofxVectorGraphics.h"

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
    
    bool isExampleData;
    ofxXmlSettings exampleDevices;
    
    ofxVectorGraphics output;
    
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
    bool signingIn;
    
    // Device Information
    int numberOfDevices;
    string targetString;
    
    // Graphing
    bool loadGraph;
    int displayGraph;
    int drawGraph;
    bool toggleGraphOverlay;
    void loadGraphData() {
        if (displayGraph == 1) {
            if (isExampleData) {
                graphNetEnergy.load("sampleData/locationEnergyUsage.xml");
                message = "Displaying Total Net Energy Usage! (example data)";
                
                graphNetEnergy.copyXmlToString(temp);
                cout << "graphNetEnergy: " << endl << temp.data() << endl;
            } else {
                ofxPeoplePower.locationEnergyUsage(XML.getValue("profile:key", "null"), XML.getValue("profile:location_id", "null"), "2", "2014-07-01T00:00:00", "null");
                graphNetEnergy = ofxPeoplePower.XML;
                message = "Displaying Total Net Energy Usage!";
                
                ofxPeoplePower.XML.copyXmlToString(temp);
                cout << "ofxPeoplePower.XML: " << endl << temp.data() << endl;
            }
            
        } else if (displayGraph == 2) {
            // TODO: add second data points
        }
    }
    
    bool displayNetEnergy;
    ofxXmlSettings graphNetEnergy;

    
    
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
        
        if (!isExampleData) {
            for (int i = 0; i < ofxPeoplePower.XML.getNumTags(tag); i++) {
                XML.addTag(tag);
                for (int j = 0; j < ofxPeoplePower.XML.getNumAttributes(tag); j++) {
                    vector<string> attributeNames;
                    
                    ofxPeoplePower.XML.getAttributeNames(tag, attributeNames);
                    XML.setAttribute(tag, attributeNames[j], ofxPeoplePower.XML.getAttribute(tag,attributeNames[j],"null", i), i);
                }
            }
        } else {
            for (int i = 0; i < exampleDevices.getNumTags(tag); i++) {
                XML.addTag(tag);
                for (int j = 0; j < exampleDevices.getNumAttributes(tag); j++) {
                    vector<string> attributeNames;
                    
                    exampleDevices.getAttributeNames(tag, attributeNames);
                    XML.setAttribute(tag, attributeNames[j], exampleDevices.getAttribute(tag,attributeNames[j],"null", i), i);
                }
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
    
    void graphCatMullPointsForXML(string parentTab,string childTab, float k) {
        
        // Find number of points
        float pts = graphNetEnergy.getNumTags(parentTab);
        
//        cout << __PRETTY_FUNCTION__ << "*** Points for " << childTab << endl;
        
        for (int i = 0; i < pts; i++) {

            graphNetEnergy.pushTag(parentTab, i - 3);
            float c1[] = {
                ofGetWidth() * ((i - 1) * (1 / pts)),
                (ofGetHeight() / 2) - (k * graphNetEnergy.getValue(childTab, 0.0))
            };
//cout << __PRETTY_FUNCTION__ << "C1[" << i << "]: (" << c1[0] << "," << c1[1] << ")" << " For value: " << graphNetEnergy.getValue(childTab, 0.0) << endl;
            graphNetEnergy.popTag();
            graphNetEnergy.pushTag(parentTab,i - 2);
            float p1[] = {
                ofGetWidth() * ((i) * (1 / pts)),
                (ofGetHeight() / 2) - (k * graphNetEnergy.getValue(childTab, 0.0))
            };
//cout << __PRETTY_FUNCTION__ << "P1[" << i << "]: (" << p1[0] << "," << p1[1] << ")" << " For value: " << graphNetEnergy.getValue(childTab, 0.0)  << endl;

            graphNetEnergy.popTag();
            graphNetEnergy.pushTag(parentTab,i - 1);
            float p2[] = {
                ofGetWidth() * ((i + 1) * (1 / pts)),
                (ofGetHeight() / 2) - (k * graphNetEnergy.getValue(childTab, 0.0))
            };
//cout << __PRETTY_FUNCTION__ << "P2[" << i << "]: (" << p2[0] << "," << p2[1] << ")" << " For value: " << graphNetEnergy.getValue(childTab, 0.0)  << endl;
            graphNetEnergy.popTag();
            graphNetEnergy.pushTag(parentTab,i);
            float c2[] = {
                ofGetWidth() * ((i + 2)* (1 / pts)),
                (ofGetHeight() / 2) - (k * graphNetEnergy.getValue(childTab, 0.0))
            };
//cout << __PRETTY_FUNCTION__ << "C2[" << i << "]: (" << c2[0] << "," << c2[1] << ")" << " For value: " << graphNetEnergy.getValue(childTab, 0.0)  << endl << endl;

            graphNetEnergy.popTag();
        
            output.curve(c1[0],c1[1],p1[0],p1[1],p2[0],p2[1],c2[0],c2[1]);
            output.circle(p1[0], p1[1], 3);
        }
    }
};
