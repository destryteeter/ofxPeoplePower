#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(255, 255, 255);
    
    //-----------
    
    // Choose server
    ofxPeoplePower.prefix = ofxPeoplePower.esp_old;
    
    // Set signed in status to false
    signedIn = false;
    
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
	TTF.drawString(" KEY  |         ACTION        ", 30, textSpacing * 2);
    TTF.drawString("------------------------------", 30, textSpacing * 3);
    TTF.drawString("  l   |         Sign In       ", 30, textSpacing * 4);
	TTF.drawString("  q   |         Sign Out      ", 30, textSpacing * 5);
	TTF.drawString("  e   |    Net Energy Usage   ", 30, textSpacing * 6);
    
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
        
        if (displayNetEnergy) {
            
            // Go to 'usages' tag
            graphNetEnergy.pushTag("response");
            graphNetEnergy.pushTag("usages");
            
            // Set constant multiplier
            float multiplier = 20;
            
            // Set number of points
            int pts = graphNetEnergy.getNumTags("usage");

            // Draw graph
            float numY = (ofGetHeight() / 4) / multiplier;
            
            output.setColor(0xEEEEEE);
            
            // Draw horizontal lines
            for(float y = 0; y < 2 * numY; y++){
                output.line(
                            0, (ofGetHeight() / 2) - (multiplier * numY) + (multiplier * y),
                            ofGetWidth(), (ofGetHeight() / 2) - (multiplier * numY) + (multiplier * y)); // ofGetHeight() * 1/3 + (y * multiplier));
            }
            
            // Draw vertical lines
            for(float x = 1; x < pts; x++){
                float lineX = ofGetWidth() * x / pts;
                output.line(
                        lineX, (ofGetHeight() / 2) - (multiplier * numY),
                        lineX, (ofGetHeight() / 2) - (multiplier * numY) + (multiplier * 2 * numY));

            }
            
            
            ofSetHexColor(0xCC0000);
            
            output.setColor(0x7e7e7e);
            output.noFill();
            
            graphCatMullPointsForXML("usage", "amount",multiplier);
            graphCatMullPointsForXML("usage", "kWh",multiplier);
            graphNetEnergy.popTag();
            graphNetEnergy.popTag();
        }
        
    } else {
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
            
            displayNetEnergy = false;
        }
        
        // Load LocationEnergyUsage
        if(key == 'e') {
            
            displayNetEnergy = true;
            ofxPeoplePower.locationEnergyUsage(XML.getValue("profile:key", "null"), XML.getValue("profile:location_id", "null"), "2", "2014-07-01T00:00:00", "null");
            
            graphNetEnergy = ofxPeoplePower.XML;
            
//            ofxPeoplePower.XML.copyXmlToString(temp);
//            cout << "ofxPeoplePower.XML: " << endl << temp.data() << endl;
            
            message = "Displaying Total Net Energy Usage!";
        }
    } else {
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
                
                // get key and asign it to XML
                ofxPeoplePower.login(username, password); // TODO: don't hardcode credentials
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
                    username = "";
                    password = "";
                    setUsername = 0;
                    setPassword = false;
                    
                } else {
                    message = "Login successful";
                    XML.saveFile();
                    signedIn = true;
                    username = "";
                    password = "";
                    setUsername = 2;
                    setPassword = true;
                }
                
                // Print XML to console
                XML.copyXmlToString(temp);
                cout << temp.data() << endl;
                
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
