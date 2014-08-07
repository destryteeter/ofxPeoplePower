#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(255, 255, 255);
    
    //-----------
    
    // Choose server
    ofxPeoplePower.prefix = ofxPeoplePower.esp_old;
    
    // Set signed in status to false
    signedIn = false;
    signingIn = false;
    
    // To use example data sign in using the username 'test' and push enter three times. (There's an issue with the shift key so the first enter adds an '@' sign.)
    // If you would like to user your own data check out Presence by People Power and look into installing a compatible smart plug.
    isExampleData = false;
    
	//the string is printed at the top of the app
	//to give the user some feedback
    message = "Loading mySettings.xml";
    
    //we load our settings file
	//if it doesn't exist we create one
    if (XML.loadFile("myProfile.xml")) {
        XML.loadFile("myProfile.xml");
        
        // Determine if user is singed in
        if (XML.getValue("profile:key","null") == "null") {
            setUsername = 0;
            setPassword = false;
            username = "";
            password = "";
            message = "Please sign in. Enter the first part of your username (before @ sign) and press return:";
        } else {
            // Determine if user has been signed out from another device.
            if (XML.getAttribute("profile:key", "expires", "") == "2030-01-01T00:00:00-05:00") {
                
                // Refresh device data to ensure it is up to date.
                ofxPeoplePower.deviceInfo(XML.getValue("profile:key","null"), XML.getValue("profile:location_id", "null"));
                
                XML.pushTag("profile");
                
                // Remove previous devices
                XML.removeTag("devices");
                XML.addTag("devices");
                
                XML.pushTag("devices");
                ofxPeoplePower.XML.pushTag("response");
                ofxPeoplePower.XML.pushTag("devices");
                
                XMLTranslate("device");
                
                // Setup device information for GUI
                numberOfDevices = XML.getNumTags("device");
                
                XML.popTag();
                XML.popTag();
                ofxPeoplePower.XML.popTag();
                ofxPeoplePower.XML.popTag();
                
                message = "Currently signed in";
                
                signedIn = true;
                setUsername = 2;
                setPassword = true;
                username = "";
                password = "";
                
            } else {
                message = "You are no longer signed in.";
            }
        }
    } else {
        setupXML();
        message = "Unable to load myProfile.xml. New file created.";
    }
    
    XML.saveFile("myProfile.xml");
    
    // Print XML to console
    XML.copyXmlToString(temp);
    cout << temp.data() << endl;
    
    //load a monospaced font
	//which we will use to show part of the xml structure
	TTF.loadFont("mono.ttf", 12);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    graphWidth = ofGetWidth() / 1.1;
    graphPosition = (ofGetWidth() - graphWidth) / 2;
}

//--------------------------------------------------------------
void ofApp::draw(){
    
	// Add the message bars at the top and bottom of the app
	// ofSetHexColor(0xDDDDDD);
	ofEnableAlphaBlending();
	ofSetColor(0, 0, 0, 200);
    output.fill();
    
    // Draw Key rectangle
	ofRect(textSpacing, textSpacing, 312, textSpacing * 9);
    ofRect(20, ofGetHeight() - 40, ofGetWidth()-40, 24);
    
    // Add our key.
    TTF.drawString("********************************", 15, textSpacing * 1.5);
    TTF.drawString("********************************", 15, textSpacing * 10.5);
	ofSetColor(240, 240, 240);
	TTF.drawString("   KEY | ACTION", 30, textSpacing * 2);
    TTF.drawString("-----------------------------", 30, textSpacing * 3);
if (!signedIn) {
    int row = 4;
    TTF.drawString("   l   | Sign In", 30, textSpacing * row);
    if (signingIn) {
    TTF.drawString("   tab | Cancel", 30, textSpacing * (row + 1));
    }
	TTF.drawString("Enter the username 'test' to ", 30, textSpacing * 8);
	TTF.drawString("display sample data.         ", 30, textSpacing * 9);
} else {
    int row = 4;
	TTF.drawString("   q   | Sign Out", 30, textSpacing * row);
    TTF.drawString("   o   | Toggle Graph Overlay", 30, textSpacing * (row + 1));
    TTF.drawString("   tab | Change data", 30, textSpacing * (row + 2));
    if (displayGraph == 1 || displayGraph == 2) {
    TTF.drawString("   0   | Show combined graphs", 30, textSpacing * (row + 3));
	TTF.drawString("   1   | Aggregated Cost", 30, textSpacing * (row + 4));
	TTF.drawString("   2   | Usage in kWh", 30, textSpacing * (row + 5));
    }
}
    
    // Add status message
    TTF.drawString("Status: " + message, 30, ofGetHeight() - 22);
    
    if (setUsername == 2 && setPassword) {
        ofSetColor(0, 0, 0, 200);
        // Display deviceInformation
        
        ofRect(312 + textSpacing * 2, textSpacing, ofGetWidth() - 312 - textSpacing * 3, textSpacing * (numberOfDevices + 1.5));
        ofSetColor(240, 240, 240);
        
        TTF.drawString("Connected", 312 + textSpacing * 3, textSpacing * 2);
        TTF.drawString("| Description", 312 + textSpacing * 3 + 100, textSpacing * 2);
        TTF.drawString("| ID", 312 + textSpacing * 3 + 350, textSpacing * 2);

        for (int i = 0; i < numberOfDevices; i ++) {

            XML.pushTag("profile");
            XML.pushTag("devices");
            
            string deviceConnected;
            string deviceDescription;
            string deviceId;
            
            XMLSetTagAttributeToString("device", "connected", i);
            deviceConnected = targetString;
            
            XMLSetTagAttributeToString("device", "desc", i);
            deviceDescription = targetString;
            
            XMLSetTagAttributeToString("device", "id", i);
            deviceId = targetString;
            
            XML.popTag();
            XML.popTag();
            
            TTF.drawString(deviceConnected , 312 + textSpacing * 3, textSpacing * (i + 3));
            TTF.drawString("| " + deviceDescription, 312 + textSpacing * 3 + 100, textSpacing * (i + 3));
            TTF.drawString("| " + deviceId, 312 + textSpacing * 3 + 350, textSpacing * (i + 3));
            
        }
        if (loadGraph) {
            
//            cout << __PRETTY_FUNCTION__ << "displayGraph: " << displayGraph << endl;
            if (displayGraph == 1) { // previously displayGraphEnergy
                
                // Go to 'response:usages' tag
                graphXML.pushTag("response");
                graphXML.pushTag("usages");
                
                // Set constant multiplier
                float multiplier = 20;
                
                // Set number of points
                int pts = graphXML.getNumTags("usage");
                
                renderGraph(pts,multiplier);
                
                if (drawGraph == 1) {
                    graphCatMullPointsForXML("usage", "amount",multiplier);
                } else if (drawGraph == 2) {
                    graphCatMullPointsForXML("usage", "kWh",multiplier);
                } else if (drawGraph == 99) {
                    graphCatMullPointsForXML("usage", "kWh",multiplier);
                    graphCatMullPointsForXML("usage", "amount",multiplier);
                    
                }

                graphXML.popTag();
                graphXML.popTag();
            } else if (displayGraph == 2) {
//                message = "This is the second graph";
                
                // Go to 'response:usages:device' tag
                graphXML.pushTag("response");
                graphXML.pushTag("usages");
                graphXML.pushTag("device");
                
                // Set constant multiplier
                float multiplier = 180;
                
                // Set number of points
                int pts = graphXML.getNumTags("usage");
                
                renderGraph(pts,multiplier);
                
                if (drawGraph == 1) {
                    graphCatMullPointsForXML("usage", "amount",multiplier);
                } else if (drawGraph == 2) {
                    graphCatMullPointsForXML("usage", "kWh",multiplier);
                } else if (drawGraph == 99) {
                    graphCatMullPointsForXML("usage", "kWh",multiplier);
                    graphCatMullPointsForXML("usage", "amount",multiplier);
                }
                
                graphXML.popTag();
                graphXML.popTag();
                graphXML.popTag();
                
            }
        }
    } else if (signingIn) {
        
        output.fill();
        ofSetColor(0, 0, 0, 200);
        ofRect(ofGetWidth() / 4, ofGetHeight() / 2 - textSpacing * 2.25, ofGetWidth() * 1/2, textSpacing * 4);
        ofSetColor(240, 240, 240);
        TTF.drawString("username: " + username, ofGetWidth() / 2 - textSpacing * 11, ofGetHeight() / 2 - textSpacing);
        TTF.drawString("password: " + password, ofGetWidth() / 2 - textSpacing * 11, ofGetHeight() / 2 + textSpacing);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (signedIn) {

        if(key == 'q') {
            
            ofFile::removeFile("myProfile.xml");
            XML.clear();
            XML.clear(); // Make sure XML is blank.
            
            ofxPeoplePower.XML.clear();
            ofxPeoplePower.XML.clear(); // Make sure XML is blank.
            
            setupXML();
            XML.saveFile("myProfile.xml");
            
            message = "Signed out!";
            
            signedIn = false;
            setUsername = 0;
            setPassword = false;
            username = "";
            password = "";
            
            displayGraph = 0;
            loadGraph = false;
        }
        
        // Load LocationEnergyUsage
        if(key == 'o') {
            if (toggleGraphOverlay == false) {
                toggleGraphOverlay = true;
            } else {
                toggleGraphOverlay = false;
            }
        }
        
        if (key == OF_KEY_TAB) {
            loadGraph = false;
            drawGraph = 99;
            if (displayGraph == 2) {
                displayGraph = 1;
            } else {
                ++displayGraph;
            }
//            ++displayGraph;
            loadGraphData();
            loadGraph = true;
        }
        if (key == '1' && loadGraph) {
            drawGraph = 1;
        }
        if (key == '2' && loadGraph) {
            drawGraph = 2;
        }
        if (key == '0' && loadGraph) {
            drawGraph = 99;
        }
        
    } else if (!signingIn) {
        if (key == 'l') {
            signingIn = true;
        }
    } else {
        if (key == OF_KEY_TAB) {
            signingIn = false;
            username = "";
            password = "";
            setPassword = false;
            setUsername = 0;
            signedIn = false;
        }
        if (setUsername == 0) {
            
            // First part
            if ((key == OF_KEY_DEL || key == OF_KEY_BACKSPACE) && username.size() > 0) {
                username = username.substr(0,username.size()-1);
            } else if (key == OF_KEY_RETURN) {
                message = "Enter the second part of your username (after @ sign) and press return:";
                
                username = username.append("@");
                setUsername = 1;
            } else {
                username.append(1,(char)key);
            }
            
        } else if (setUsername == 1) {
            
            // First part
            if ((key == OF_KEY_DEL || key == OF_KEY_BACKSPACE) && username.size() > 0) {
                username = username.substr(0,username.size()-1);
            } else if (key == OF_KEY_RETURN) {
                
                message = "Enter your password: ";
                setUsername = 2;
            } else {
                username.append(1,(char)key);
            }
            
        } else if (setUsername == 2 && !setPassword) {
            
            // Third Part
            
            if ((key == OF_KEY_DEL || key == OF_KEY_BACKSPACE) && password.size() > 0) {
                password = password.substr(0,password.size()-1);
            } else if (key == OF_KEY_RETURN) {
                message = "Signing in as " + username;
                
                setPassword = true;
                
                if  (username == "test@") {
                    
                    isExampleData = true;
                    
                    exampleDevices.load("sampleData/deviceInfo.xml");
                    
                    
                    XML.pushTag("profile");
                    
                    // Remove previous devices
                    XML.removeTag("devices");
                    XML.addTag("devices");
                    
                    XML.pushTag("devices");
                    exampleDevices.pushTag("response");
                    exampleDevices.pushTag("devices");
                    
                    XMLTranslate("device");
                    
                    // Setup device information for GUI
                    numberOfDevices = XML.getNumTags("device");
                    
                    XML.popTag();
                    XML.popTag();
                    exampleDevices.popTag();
                    exampleDevices.popTag();
                    
                    
                    
                    message = "Login successful";
                    XML.saveFile();
                    signedIn = true;
                    username = "";
                    password = "";
                    setUsername = 2;
                    setPassword = true;

                } else {
                
                    isExampleData = false;
                    
                // get key and asign it to XML
                ofxPeoplePower.login(username, password);
                    
                    ofxPeoplePower.XML.copyXmlToString(temp);
                    cout << temp.data() << endl;
                    
                XML.pushTag("profile",0);
                
                XML.setValue("key", ofxPeoplePower.XML.getValue("response:key","null"));
                XML.setAttribute("key", "expires", ofxPeoplePower.XML.getValue("response:keyExpire","null"), 0);
                
                // Get user_id and location_id and asign it to XML
                ofxPeoplePower.user(XML.getValue("key","null"));
                XML.setValue("user_id", ofxPeoplePower.XML.getAttribute("response:user", "id", 0));
                XML.setValue("location_id", ofxPeoplePower.XML.getAttribute("response:locations:location", "id", 0));
                XML.popTag();
                
                // Get user device information
                ofxPeoplePower.deviceInfo(XML.getValue("profile:key","null"), XML.getValue("profile:location_id", "null"));
                
                XML.pushTag("profile");
                
                // Remove previous devices
                XML.removeTag("devices");
                XML.addTag("devices");
                
                XML.pushTag("devices");
                ofxPeoplePower.XML.pushTag("response");
                ofxPeoplePower.XML.pushTag("devices");
                
                XMLTranslate("device");
                
                // Setup device information for GUI
                numberOfDevices = XML.getNumTags("device");
                
                XML.popTag();
                XML.popTag();
                ofxPeoplePower.XML.popTag();
                ofxPeoplePower.XML.popTag();
                
                // Return status message
                if (XML.getValue("profile:key","null") == "null") {
                    message = "Login failed, please try again!";
                    XML.clear();
                    setupXML();
                    username = "";
                    password = "";
                    setUsername = 0;
                    setPassword = false;
                    
                } else {
                    message = "Login successful.  Press TAB to graph your data";
                    XML.saveFile();
                    signedIn = true;
                    username = "";
                    password = "";
                    setUsername = 2;
                    setPassword = true;
                    
                }
                }
                // Print XML to console
                XML.copyXmlToString(temp);
                cout << temp.data() << endl;
                signingIn = false;
                
            } else {
                password.append(1,(char)key);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
