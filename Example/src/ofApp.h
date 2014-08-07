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
    ofxXmlSettings graphXML;
    float graphWidth = ofGetWidth() / 1.1;
    void loadGraphData() {
        cout << __PRETTY_FUNCTION__ << "displayGraph: " << displayGraph << endl;
        if (displayGraph == 1) {
            message = "Displaying Location Energy Usage!";
            if (isExampleData) {
                graphXML.loadFile("sampleData/locationEnergyUsage.xml");
                message.append(" (example data)");
//                graphXML.copyXmlToString(temp);
//                cout << "graphXML: " << endl << temp.data() << endl;
            } else {
                ofxPeoplePower.locationEnergyUsage(XML.getValue("profile:key", "null"), XML.getValue("profile:location_id", "null"), "2", "2014-07-01T00:00:00", "null");
                graphXML = ofxPeoplePower.XML;
//                ofxPeoplePower.XML.copyXmlToString(temp);
//                cout << "ofxPeoplePower.XML: " << endl << temp.data() << endl;
            }
        } else if (displayGraph == 2) {  // deviceEnergyUsage
            message = "Displaying Device Energy Usage!";
            if (isExampleData) {
                graphXML.load("sampleData/deviceEnergyUsage2.xml");
                message.append(" (example data)");
            } else {
                ofxPeoplePower.deviceEnergyUsage(XML.getValue("profile:key", "null"), XML.getValue("profile:location_id", "null"), "1", "2014-08-01T00:00:00", "null", "MON31693");
                graphXML = ofxPeoplePower.XML;
//                ofxPeoplePower.XML.copyXmlToString(temp);
//                cout << "ofxPeoplePower.XML: " << endl << temp.data() << endl;
            }
        }
    }
    void renderGraph(int pts, int multiplier) {
        // Draw graph
        float numY = (ofGetHeight() / 100); // 4 / multiplier);
//cout << __PRETTY_FUNCTION__ << "numY: " << numY << endl;
        
        output.setColor(0xEEEEEE);
        
        // Define outer edges
        float lineX[] = {(graphWidth * 1 / (pts - 1)), graphWidth}; // TODO: Center graph
        float lineY[] = {ofGetHeight() / 4, ofGetHeight() / 2};
        
        // Draw border
        output.noFill();
        output.rect(lineX[0], lineY[0], lineX[1], lineY[1]);
        output.fill();
        
        // Draw positive horizontal lines
        for(float y = 0; y < numY; y++){ // TODO: subdivide lines when space allows
//            float lineY = (ofGetHeight() / 2) - (multiplier * (numY - .5)) + (multiplier * y);
            float pointY = (ofGetHeight() / 2) - (multiplier * numY) + (multiplier * y);
            
            if (pointY >= ofGetHeight() / 4) {
                output.line(lineX[0], pointY, lineX[1] + (graphWidth * 1 / (pts - 1)), pointY);
            }
//cout << __PRETTY_FUNCTION__ << "lineY[" << y << "]: " << lineY << endl;
        }
        
        // Draw negaiteve horizontal lines
        for(float y = 0; y < numY + 1; y++){
            float pointY = (ofGetHeight() / 2) + (multiplier * y);
//            float lineX[] = {(graphWidth * 1 / (pts - 1)), graphWidth + (graphWidth * 1 / (pts - 1))};
            if (pointY <= (ofGetHeight() * 3 / 4)) {
                output.line(lineX[0], pointY, lineX[1] + (graphWidth * 1 / (pts - 1)), pointY);
            }
        }
        
        // Draw vertical lines
        for(float x = 0; x < pts; x++){
            float pointX = graphWidth * x / (pts - 1) + (graphWidth * 1 / (pts - 1));
            output.line(pointX, lineY[0], pointX, lineY[1] + (ofGetHeight() / 4));
        }
    }
    
    bool displayNetEnergy;

    
    
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
        output.setColor(0x7e7e7e);
        output.noFill();
        
        // Find number of points
        float pts = graphXML.getNumTags(parentTab) - 1;

       cout << __PRETTY_FUNCTION__ << "*** Points for " << childTab << endl;
        
        for (int i = 0; i < pts; i++) {

            graphXML.pushTag(parentTab, i - 1);
            float c1[] = {
                graphWidth * ((i - 1) * (1 / pts)) + (graphWidth * 1 / pts),
                (ofGetHeight() / 2) - (k * graphXML.getValue(childTab, 0.0))
            };
//cout << __PRETTY_FUNCTION__ << "C1[" << i << "]: (" << c1[0] << "," << c1[1] << ")" << " For value: " << graphXML.getValue(childTab, 0.0) << endl;
            graphXML.popTag();
            graphXML.pushTag(parentTab,i);
            float p1[] = {
                graphWidth * ((i) * (1 / pts)) + (graphWidth * 1 / pts),
                (ofGetHeight() / 2) - (k * graphXML.getValue(childTab, 0.0))
            };
//cout << __PRETTY_FUNCTION__ << "P1[" << i << "]: (" << p1[0] << "," << p1[1] << ")" << " For value: " << graphXML.getValue(childTab, 0.0)  << endl;

            graphXML.popTag();
            graphXML.pushTag(parentTab,i + 1);
            float p2[] = {
                graphWidth * ((i + 1) * (1 / pts)) + (graphWidth * 1 / pts),
                (ofGetHeight() / 2) - (k * graphXML.getValue(childTab, 0.0))
            };
//cout << __PRETTY_FUNCTION__ << "P2[" << i << "]: (" << p2[0] << "," << p2[1] << ")" << " For value: " << graphXML.getValue(childTab, 0.0)  << endl;
            graphXML.popTag();
            // Check to see if it is the last tag
            if (graphXML.tagExists(parentTab,i + 2)) {
                graphXML.pushTag(parentTab,i + 2);
            } else {
                graphXML.pushTag(parentTab,i + 1);
                output.circle(p2[0], p2[1], 3);
            }
            float c2[] = {
                graphWidth * ((i + 2)* (1 / pts)) + (graphWidth * 1 / pts),
                (ofGetHeight() / 2) - (k * graphXML.getValue(childTab, 0.0))
            };
//cout << __PRETTY_FUNCTION__ << "C2[" << i << "]: (" << c2[0] << "," << c2[1] << ")" << " For value: " << graphXML.getValue(childTab, 0.0)  << endl << endl;

            graphXML.popTag();
        
            output.curve(c1[0],c1[1],p1[0],p1[1],p2[0],p2[1],c2[0],c2[1]);
            output.circle(p1[0], p1[1], 3);
        }
    }
};
