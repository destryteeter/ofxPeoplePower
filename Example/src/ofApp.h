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
    
    bool setUsername;
    bool setPassword;
    
    bool signedIn;
    bool signingIn;
    
    bool isLoggingOut;
    
    bool printOnce;
    
    // Device Information
    int numberOfDevices;
    string targetString;
    
    // Graphing
    bool displayNetEnergy;
    bool loadGraph;
    int displayGraph;
    int drawGraph;
    bool toggleGraphOverlay;
    ofxXmlSettings graphXML;
    float graphWidth;
    float graphPosition;
    float yMax;
    float yMin;
    float k;
    
    // Api Input
    bool receivesAggeregateFlag;
    int AggregateFlag;
    bool receivesStartDate;
    string startDate;
    bool receivesEndDate;
    string endDate;
    bool receivesDevice;
    int device;
    
    bool receiveUserInput;
    string userInputFlag;
    string userInput;
    
    void resetReceivableFlags() {
        receivesAggeregateFlag = false;
        receivesStartDate = false;
        receivesEndDate = false;
        receivesDevice = false;
        receiveUserInput = false;
    }
    
    void resetUser() {
        
        // Delete myProfile.xml
        ofFile::removeFile("myProfile.xml");
        XML.clear();
        XML.clear(); // Make sure XML is blank.
        
        ofxPeoplePower.XML.clear();
        ofxPeoplePower.XML.clear(); // Make sure XML is blank.
        
        // Create and save a black myProfile.xml
        setupXML();
        XML.saveFile("myProfile.xml");
        
        signedIn = false;
        setUsername = false;
        setPassword = false;
        username = "";
        password = "";
        
        displayGraph = 0;
        loadGraph = false;
    }
    
    void setDefaultApi() {
        AggregateFlag = 1;
        startDate = "2014-08-05T00:00:00"; // TODO: call start date dynamically based off of today's date
        endDate = "null";
        device = 0;
    }
    
    void loadGraphData() {
        if (displayGraph == 1) {
            message = "Displaying Location Energy Usage!";
            if (isExampleData) {
                graphXML.loadFile("sampleData/locationEnergyUsage.xml");
                message.append(" (example data)");
            } else {
                
                // Set receivable flags
                receivesAggeregateFlag = true;
                receivesStartDate = true;
                receivesEndDate = true;
                
                ofxPeoplePower.locationEnergyUsage(XML.getValue("profile:key", "null"), XML.getValue("profile:location_id", "null"), ofToString(AggregateFlag), startDate, endDate);
                graphXML = ofxPeoplePower.XML;
            }
        } else if (displayGraph == 2) {
            message = "Displaying Device Energy Usage!";
            if (isExampleData) {
                graphXML.load("sampleData/deviceEnergyUsage2.xml");
                message.append(" (example data)");
            } else {
                
                // Set receivable flags
                receivesAggeregateFlag = true;
                receivesStartDate = true;
                receivesEndDate = true;
                receivesDevice = true;
                
                XML.pushTag("profile");
                XML.pushTag("devices");
//                XML.pushTag("device",device);
                string deviceId = XML.getAttribute("device","id","null",device);
                XML.popTag();
                XML.popTag();
                
                ofxPeoplePower.deviceEnergyUsage(XML.getValue("profile:key", "null"), XML.getValue("profile:location_id", "null"), ofToString(AggregateFlag), startDate, endDate, deviceId);
                graphXML = ofxPeoplePower.XML;
            }
        }
#ifdef DEBUG
        if (isExampleData) {
            graphXML.copyXmlToString(temp);
        }
        else {
            ofxPeoplePower.XML.copyXmlToString(temp);
        }
        cout << __PRETTY_FUNCTION__ << endl << temp.data() << endl;
#endif
    }
    void renderGraph(int pts, float multiplier) {
        
        // Define line height
        float numY = (ofGetHeight() / 80);
        
        // Define outer edges
        float lineX[] = {graphPosition, graphWidth};
        float lineY[] = {ofGetHeight() / 4, ofGetHeight() / 2};
        
        // Draw positive horizontal lines
        for(float y = 0; y < numY; y++){
            float pointY = (ofGetHeight() / 2) + (multiplier * y) - (multiplier * numY);
            float pointYPrevious = (ofGetHeight() / 2) + (multiplier * y) - (multiplier * (numY - 1));
            float pointYDelta = pointYPrevious - pointY;
            if (pointY >= ofGetHeight() / 4) {
                output.setColor(0xDDDDDD);
                output.line(lineX[0], pointY, lineX[1] + graphPosition, pointY);
                if (multiplier >= 100) {
                    for (int j = 1; j < 10; j++) {
                        float pointYDivide = (pointYDelta * j) / 10;
                        output.setColor(0xEEEEEE);
                        output.line(lineX[0], pointY + pointYDivide, lineX[1] + graphPosition, pointY + pointYDivide);
                    }
                }
            }
        }
        
        // Draw negaiteve horizontal lines
        for(float y = 0; y < numY; y++){
            float pointY = (ofGetHeight() / 2) - (multiplier * y) + (multiplier * numY);
            float pointYPrevious = (ofGetHeight() / 2) - (multiplier * y) + (multiplier * (numY - 1));
            float pointYDelta = pointY - pointYPrevious;
            if (pointY <= (ofGetHeight() * 3 / 4)) {
                output.setColor(0xDDDDDD);
                output.line(lineX[0], pointY, lineX[1] + graphPosition, pointY);
                if (multiplier >= 100) {
                    for (int j = 1; j < 10; j++) {
                        float pointYDivide = (pointYDelta * j) / 10;
                        output.setColor(0xEEEEEE);
                        output.line(lineX[0], pointY - pointYDivide, lineX[1] + graphPosition, pointY - pointYDivide);
                    }
                }
            }
        }
        
        // Draw vertical lines
        for(float x = 0; x < pts; x++){
            float pointX = graphWidth * x / (pts - 1) + graphPosition;
            output.setColor(0xDDDDDD);
            output.line(pointX, lineY[0], pointX, lineY[1] + (ofGetHeight() / 4));
        }
        
        // Draw border
        output.setColor(0x000000); // TODO: set to a darker color
        output.noFill();
        output.rect(lineX[0], lineY[0], lineX[1], lineY[1]);
        output.fill();
        
        // Draw center line with tics
        output.setColor(0xBBBBBB);
        output.line(lineX[0], ofGetHeight() / 2, lineX[1] + graphPosition, ofGetHeight() / 2);
        for(float x = 1; x < pts - 1; x++){
            float pointX = graphWidth * x / (pts - 1) + graphPosition;
            output.line(pointX, (ofGetHeight() / 2) - 5, pointX, (ofGetHeight() / 2) + 5);
        }
    }
    
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
        for (int j = 0; j < XML.getNumAttributes(tag); j++) {
            vector<string> attributeNames;
            
            XML.getAttributeNames(tag, attributeNames);
            if (attributeNames[j] == attribute) {
                targetString = XML.getAttribute(tag,attributeNames[j],"null", k);
            }
        }
    }
    
    void graphCatMullPointsForXML(string parentTab,string childTab) {
        output.setColor(0x7e7e7e);
        output.noFill();
        
        // Find number of points
        float pts = graphXML.getNumTags(parentTab) - 1;
        
        // Find appropriate multiplier
        for (int i = 0; i < pts; i++) {
            graphXML.pushTag(parentTab,i);
            if (graphXML.getValue(childTab, 0.0) > yMax) {
                yMax = (graphXML.getValue(childTab, 0.0));
            }
            if (graphXML.getValue(childTab, 0.0) < yMin) {
                yMin = (graphXML.getValue(childTab, 0.0));
            }
            graphXML.popTag();
            
#ifdef DEBUG
            if (printOnce) {
                cout << __PRETTY_FUNCTION__ << "yMax: " << yMax << "|| yMin: " << yMin << endl;
            }
#endif
        }
        
        if (yMax > -yMin) {
            k = (ofGetHeight()/2 / (yMax * 2.2));
        } else {
            k = (ofGetHeight()/2 / (yMin * 2.2));
        }
        
#ifdef DEBUG
        if (printOnce) {
            cout << __PRETTY_FUNCTION__ << "k: " << "(" << ofGetHeight()/2 << " / (" << yMax * 10 << "))" << endl;
        }
#endif

        
        for (int i = 0; i < pts; i++) {
            
            graphXML.pushTag(parentTab, i - 1);
            float c1[] = {
                (graphWidth * ((i - 1) * (1 / pts))) + graphPosition,
                (ofGetHeight() / 2) - (k * graphXML.getValue(childTab, 0.0))
            };
            graphXML.popTag();
            graphXML.pushTag(parentTab,i);
            float p1[] = {
                (graphWidth * ((i) * (1 / pts))) + graphPosition,
                (ofGetHeight() / 2) - (k * graphXML.getValue(childTab, 0.0))
            };
            graphXML.popTag();
            graphXML.pushTag(parentTab,i + 1);
            float p2[] = {
                (graphWidth * ((i + 1) * (1 / pts))) + graphPosition,
                (ofGetHeight() / 2) - (k * graphXML.getValue(childTab, 0.0))
            };
            graphXML.popTag();
            
            // If tag exists use it, else use the previous and mark it
            if (graphXML.tagExists(parentTab,i + 2)) {
                graphXML.pushTag(parentTab,i + 2);
#ifdef DEBUG
                if (printOnce) {
//                    cout << __PRETTY_FUNCTION__ << "p1[" << i << "]: (" << p1[0] << ", " << p1[1] << ")" << endl;
                }
#endif
            }
            else {
                graphXML.pushTag(parentTab,i + 1);
                output.circle(p2[0], p2[1], 1);
#ifdef DEBUG
            if (printOnce) {
//                cout << __PRETTY_FUNCTION__ << "p2[" << i << "]: (" << p2[0] << ", " << p2[1] << ")" << endl;
            }
#endif
            }
            float c2[] = {
                (graphWidth * ((i + 2)* (1 / pts))) + graphPosition,
                (ofGetHeight() / 2) - (k * graphXML.getValue(childTab, 0.0))
            };
            
            graphXML.popTag();
            
            output.curve(c1[0],c1[1],p1[0],p1[1],p2[0],p2[1],c2[0],c2[1]);
            output.circle(p1[0], p1[1], 1);

        }
    }
};
